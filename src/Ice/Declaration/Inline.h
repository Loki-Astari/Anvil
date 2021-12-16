#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_INLINE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_INLINE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include "Common.h"
#include "Container.h"
#include "Namespace.h"
#include "Types.h"

namespace ThorsAnvil::Anvil::Ice
{


// By default only object need runtime storage.
// So most types simply return false.
template<typename T>
inline
bool doesDeclNeedRuntimeStorage(Container&, Decl const&)
{
    return false;
}
template<>
inline
bool doesDeclNeedRuntimeStorage<Object>(Container& container, Decl const& decl)
{
    Object const& object = dynamic_cast<Object const&>(decl);
    // If we don't store functions and thus is a function then return false.
    // Otherwise return true as:
    //      A: This is not a function so needs storage in the local context.
    //      B: Is a function and the local context says it needs to store it.
    return (!container.storeFunctionsInContainer() && object.getType().declType() == DeclType::Func)
                ? false
                : true;
}

template<typename T, typename... Args>
inline T& Container::add(Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(args...));
    auto& location = names[decl->declName()];
    if (doesDeclNeedRuntimeStorage<T>(*this, *decl))
    {
        objectId[decl->declName()] = nextObjectId++;
    }
    location = std::move(decl);
    return dynamic_cast<T&>(*location);
}
inline std::pair<bool, NameRef> Container::get(std::string const& name) const
{
    auto find = names.find(name);
    return {find != names.end(), find};
}

inline Namespace::Namespace(std::string const& name)
    : DeclContainer<Decl>(name)
{
    // Move to the globalscope.
    static Void  voidType;

    Func& constructType = add<Func>("$Constructor", ParamList{}, voidType);
    Func& destructType  = add<Func>("$Destructor",  ParamList{}, voidType);

    add<Object>("$constructor", constructType);
    add<Object>("$destructor",  destructType);

    add<CodeBlock>("$constructor$Code");
    add<CodeBlock>("$destructor$Code");
}

inline Class::Class(std::string const& name)
    : DeclContainer<Type>(name)
{
    // Move to the globalscope.
    static Void  voidType;

    Func& constructType = add<Func>("$Constructor", ParamList{}, voidType);
    Func& destructType  = add<Func>("$Destructor",  ParamList{}, voidType);

    add<Object>("$constructor", constructType);
    add<Object>("$destructor",  destructType);

    add<CodeBlock>("$constructor$Code");
    add<CodeBlock>("$destructor$Code");
}

}

#endif
