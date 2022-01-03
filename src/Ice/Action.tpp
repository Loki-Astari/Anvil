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
    extended << "Error: ";
    (extended << ... << args);
    extended << "\n" << *this;
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

inline std::string identifierType(std::string const& key)
{
    if (std::islower(key[0]))
    {
        return "Object";
    }
    if (key.length() <= 3 || key.find('_') != std::string::npos)
    {
        return "Namespace";
    }
    return "Type";
}


template<typename T, typename V, typename E>
T& Action::getRefFromView(Identifier const& key, V view, E&& genError)
{
    for (auto const& scope: view)
    {
        auto find = scope.get().get(key);
        if (find.first)
        {
            Decl& decl = *find.second->second;
            T& value = dynamic_cast<T&>(decl);
            return value;
        }
    }
    genError(key);
    // The genError should never return.
    // It is supposed to generate a call to error() with a detailed
    // error message specific to the caller.
    throw domain_error("Error occurred but no exception generated.");
}
// -----------

template<typename T>
Id<T> Action::getNameFromScopeStack(IdentifierId id)
{
    IdentifierAccess    idAccess(storage, id);
    T& value = getRefFromScopeStack<T>(idAccess);
    return storage.add<Ref<T>>(value);
}
template<typename T>
T& Action::getRefFromScopeStack(Identifier const& id)
{
    if (id == "::")
    {
        Scope&  global = currentScope.front();
        return dynamic_cast<T&>(global);
    }
    return getRefFromView<T>(id, make_View<Reverse>(currentScope), [&](std::string const& key)
        {
            error("Invalid Identifier Name: ", key, " (", identifierType(key), ")\n",
                  "\n",
                  "The Identifier >", key, "< can not be found in the current scope stack (see 'Parser State' below)\n"
                 );
        });
}
// -----------

template<typename T>
Id<T> Action::getNameFromScope(ScopeId scopeId, IdentifierId id)
{
    IdentifierAccess    idAccess(storage, id);
    ScopeAccess         scopeAccess(storage, scopeId);
    ScopeRef            scope = ScopeRef(scopeAccess);
    T&                  value = getRefFromScope<T>(scope, idAccess);
    return storage.add<Ref<T>>(value);
}
template<typename T>
T& Action::getRefFromScope(ScopeRef const& scope, Identifier const& id)
{
    return getRefFromView<T>(id, make_View<Reverse>(&scope, &scope+1), [&](std::string const& key)
        {
            error("Invalid Identifier Name: ", key, " (", identifierType(key), ")\n",
                  "\n",
                  "The Identifier >", key, "< can not be found in >", scope.get().declName(true), "<\n"
                 );
        });
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
    Base<Dst>& result   = topScope.add<Dst>(this, static_cast<Param&>(IdAccess<Param>{storage, id})...);
    return storage.add(Ref<Base<Dst>>{result});
}

template<typename Dst, typename... Param>
Dst& Action::getOrAddDeclToScope(Scope& scope, std::string declName, Param... param)
{
    auto find = scope.get(declName);
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
    Dst& addedScope = scope.add<Dst>(this, std::move(declName), std::move(param)...);
    return addedScope;
}


}

#endif
