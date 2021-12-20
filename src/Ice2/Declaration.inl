// Included from Declaration.h

// By default only object need runtime storage.
// So most types simply return false.
template<typename T>
inline
bool doesDeclNeedRuntimeStorage(Scope&, Decl const&)
{
    return false;
}

/*
template<>
inline
bool doesDeclNeedRuntimeStorage<Object>(Scope& scope, Decl const& decl)
{
    Object const& object = dynamic_cast<Object const&>(decl);
    // If we don't store functions and thus is a function then return false.
    // Otherwise return true as:
    //      A: This is not a function so needs storage in the local context.
    //      B: Is a function and the local context says it needs to store it.
    return (!scope.storeFunctionsInContainer() && object.getType().declType() == DeclType::Func)
                ? false
                : true;
}
*/

template<typename T, typename... Args>
inline T& Scope::add(Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(args...));
    auto& location = members[decl->declName()];
    if (doesDeclNeedRuntimeStorage<T>(*this, *decl))
    {
        objectId[decl->declName()] = nextObjectId++;
    }
    location = std::move(decl);
    return dynamic_cast<T&>(*location);
}
