#ifndef TEST_COMPILE_ACTION
#define TEST_COMPILE_ACTION

#include "Semantic.h"

#include <sstream>


#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_TRUE(check);         \
                                        } catch(...) {                  \
                                            std::cerr << "TRUE: FAIL\n";\
                                            std::cerr << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }
#define EXPECT_FALSE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_FALSE(check);        \
                                        } catch(...) {                  \
                                            std::cerr << "FALSE FAIL\n";\
                                            std::cerr << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }
#define EXPECT_THROW_OR_DEBUG(check, msg, output) \
                                     {                                  \
                                        try {                           \
                                            check;                      \
                                            std::cerr << "THROW FAIL (Throw expected but did not happen)\n";\
                                            std::cerr << output.rdbuf();\
                                        } catch(std::exception const& e)\
                                        {                               \
                                            std::string error(e.what());\
                                            if (error.find(msg) == std::string::npos) {\
                                                std::cerr << "THROW FAIL (Throw happened but not expected message)\n" \
                                                          << error << "\n"  \
                                                          << output.rdbuf();\
                                                throw;                  \
                                            }                           \
                                        }                               \
                                      }

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}

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
