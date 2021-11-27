#ifndef TEST_SEMANTIC_H
#define TEST_SEMANTIC_H

#include "Semantic.h"

template<typename T>
T& getType(ThorsAnvil::Anvil::Ice::Scope& scope, std::string const& name)
{
    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
        throw std::runtime_error("Not Found");
    }

    ThorsAnvil::Anvil::Ice::Decl*  decl         = find.second->second.get();
    return *(dynamic_cast<T*>(find.second->second.get()));
}

template<typename T, typename... Args>
T& getType(ThorsAnvil::Anvil::Ice::Scope& scope, std::string const& name, Args&... path)
{
    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
        throw std::runtime_error("Not Found");
    }

    ThorsAnvil::Anvil::Ice::Decl*  decl         = find.second->second.get();

    ThorsAnvil::Anvil::Ice::Class* classDecl    = dynamic_cast<ThorsAnvil::Anvil::Ice::Class*>(decl);
    if (classDecl)
    {
        return getType<T>(dynamic_cast<ThorsAnvil::Anvil::Ice::Scope&>(*classDecl), path...);
    }
    ThorsAnvil::Anvil::Ice::Namespace* namespaceDecl    = dynamic_cast<ThorsAnvil::Anvil::Ice::Namespace*>(decl);
    if (namespaceDecl)
    {
        return getType<T>(dynamic_cast<ThorsAnvil::Anvil::Ice::Scope&>(*namespaceDecl), path...);
    }
    throw std::runtime_error("Not a valid Scope Path");
}

#endif
