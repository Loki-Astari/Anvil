#ifndef THORSANVIL_ANVIL_ICE_ACTION_TPP
#define THORSANVIL_ANVIL_ICE_ACTION_TPP

#include "Storage.h"
#include "StorageAccess.h"
#include "Utility/View.h"

namespace ThorsAnvil::Anvil::Ice
{

template<typename... Args>
void Action::log(Args const&... args) const
{
    (output << ... << args);
}

template<typename... Args>
void Action::error(Args const&... args) const
{
    std::stringstream extended;
    extended << "Error: '";
    (extended << ... << args);
    extended << "'\n" << *this;
    throw std::runtime_error(extended.str());
}

template<typename T>
ListId<T> Action::listCreate()
{
    return storage.add<List<T>>(List<T>{});
}

template<typename T, typename S>
ListId<T> Action::listAppend(ListId<T> listId, Id<S> id)
{
    ListAccess<T>   listAccess(storage, listId);
    IdAccess<S>     objectAccess(storage, id);

    List<T>&        list = listAccess;
    S&              object = objectAccess;

    list.emplace_back(object);
    return listAccess.reuse();
}

template<typename T, typename V>
Id<T> Action::getNameFromView(IdentifierId id, V view)
{
    using Ref = std::reference_wrapper<T>;
    //ScopePrinter scope("getTypeFromName");
    IdentifierAccess    access(storage, id);

    for (auto const& scope: view)
    {
        auto find = scope.get().get(access);
        if (find.first)
        {
            Decl& decl = *find.second->second;
            T& value = dynamic_cast<T&>(decl);
            return storage.add<Ref>(Ref{value});
        }
    }
    error("Invalid Type Name: ", static_cast<std::string>(access));
}

template<typename T>
Id<T> Action::getNameFromScopeStack(IdentifierId id)
{
    if (storage.get<Identifier>(id.value) == "::")
    {
        Scope&  global = currentScope.front();
        T& result = dynamic_cast<T&>(global);
        return storage.add<Ref<T>>(result);
    }
    return getNameFromView<T>(id, make_View<Reverse>(currentScope));
}
template<typename T>
Id<T> Action::getNameFromScope(ScopeId scopeId, IdentifierId id)
{
    ScopeAccess     scopeAccess(storage, scopeId);
    ScopeRef        scope = ScopeRef(scopeAccess);
    return getNameFromView<T>(id, make_View<Reverse>(&scope, &scope+1));
}
// -------

template<typename T, typename V>
T& Action::getRefFromView(Identifier const& id, V view)
{
    for (auto const& scope: view)
    {
        auto find = scope.get().get(id);
        if (find.first)
        {
            Decl& decl = *find.second->second;
            T& value = dynamic_cast<T&>(decl);
            return value;
        }
    }
    error("Invalid Type Name: ", id);
}
template<typename T>
T& Action::getRefFromScopeStack(Identifier const& id)
{
    if (id == "::")
    {
        Scope&  global = currentScope.front();
        return dynamic_cast<T&>(global);
    }
    return getRefFromView<T>(id, make_View<Reverse>(currentScope));
}
template<typename T>
T& Action::getRefFromScope(ScopeRef const& scope, Identifier const& id)
{
    return getRefFromView<T>(id, make_View<Reverse>(&scope, &scope+1));
}
// --------

template<typename From, typename To>
Id<To> Action::convert(Id<From> id)
{
    using ToStoreType = typename IdTraits<To>::AccessType;
    IdAccess<From>  access(storage, id);
    From&  fromValue = access;
    To&    toType = dynamic_cast<To&>(fromValue);
    return storage.add<ToStoreType>(ToStoreType{toType});
}

template<typename Dst, typename... Param>
Id<Base<Dst>> Action::addDeclToScope(Id<Param>... id)
{
    Scope&     topScope = currentScope.back();
    Base<Dst>& result   = topScope.add<Dst>(*this, static_cast<Param&>(IdAccess<Param>{storage, id})...);
    return storage.add(Ref<Base<Dst>>{result});
}

template<typename Dst, typename... Param>
Dst& Action::getOrAddDeclToScope(std::string declName, Param... param)
{
    Scope& topScope = currentScope.back();
    auto find = topScope.get(declName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        Dst*        result = dynamic_cast<Dst*>(foundDeclWithSameName);
        if (result == nullptr)
        {
            error("Object With name already exists: ", declName);
        }
        return *result;
    }
    Dst& addedScope = topScope.add<Dst>(*this, std::move(declName), std::move(param)...);
    return addedScope;
}

template<typename Dst, typename... Param>
ObjectFunction& Action::addFunctionToScope(std::string name, Param&&... param)
{
    ObjectOverload& objectOverload = getOrAddDeclToScope<ObjectOverload>(name);

    std::unique_ptr<Dst> func = std::make_unique<Dst>(this, std::move(name), std::forward<Param>(param)...);
    ObjectFunction&      result = *func;
    objectOverload.addOverload(std::move(func));
    return result;
}

}

#endif
