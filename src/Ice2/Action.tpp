#ifndef THORSANVIL_ANVIL_ICE_ACTION_TPP
#define THORSANVIL_ANVIL_ICE_ACTION_TPP

#include "Storage.h"
#include "ParserTypes.h"
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

template<typename Dst, typename Param1>
Id<Base<Dst>> Action::addObjectToScope1(Id<Param1> id)
{
    IdAccess<Param1>    param1Access(storage, id);
    Param1&             param1 = param1Access;

    Scope&     topScope = currentScope.back();
    Base<Dst>& result   = topScope.add<Dst>(*this, param1);
    return storage.add(Ref<Base<Dst>>{result});
}
template<typename Dst, typename Param1, typename Param2>
Id<Base<Dst>> Action::addObjectToScope2(Id<Param1> id1, Id<Param2> id2)
{
    IdAccess<Param1>    param1Access(storage, id1);
    IdAccess<Param2>    param2Access(storage, id2);
    Param1&             param1 = param1Access;
    Param2&             param2 = param2Access;

    Scope&     topScope = currentScope.back();
    Base<Dst>& result   = topScope.add<Dst>(*this, param1, param2);
    return storage.add(Ref<Base<Dst>>{result});
}

template<typename T>
T& Action::getOrAddScope(Scope& topScope, std::string scopeName)
{
    auto find = topScope.get(scopeName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        return dynamic_cast<T&>(*foundDeclWithSameName);
    }
    T& addedScope = topScope.add<T>(*this, std::move(scopeName));
    addedScope.setPath(getCurrentScopeFullName());
    return addedScope;
}

}

#endif
