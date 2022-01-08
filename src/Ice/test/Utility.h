#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include "Parser.h"
#include "Lexer.h"
#include "Storage.h"
#include "Declaration.h"
#include "Semantic.h"
#include "Generator.h"
#include "GlobalScopeInit.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <sstream>
// #include <fstream>

struct FacadeCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Namespace       globalScope;
    ThorsAnvil::Anvil::Ice::Storage         storage;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::GlobalScopeInit init;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    FacadeCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, globalScope, storage, output)
        , init(action, globalScope)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

struct SemanticCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer               lexer;
    ThorsAnvil::Anvil::Ice::Namespace           globalScope;
    ThorsAnvil::Anvil::Ice::Storage             storage;
    ThorsAnvil::Anvil::Ice::NamespaceDecOrder   order;
    ThorsAnvil::Anvil::Ice::Semantic            action;
    ThorsAnvil::Anvil::Ice::GlobalScopeInit     init;
    ThorsAnvil::Anvil::Ice::Parser              parser;

    SemanticCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, order, globalScope, storage, output)
        , init(action, globalScope)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

struct GeneratorCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer               lexer;
    ThorsAnvil::Anvil::Ice::Namespace           globalScope;
    ThorsAnvil::Anvil::Ice::Storage             storage;
    ThorsAnvil::Anvil::Ice::NamespaceDecOrder   order;
    ThorsAnvil::Anvil::Ice::Semantic            action;
    ThorsAnvil::Anvil::Ice::GlobalScopeInit     init;
    ThorsAnvil::Anvil::Ice::Parser              parser;
    ThorsAnvil::Anvil::Ice::Generator           generator;

    GeneratorCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, order, globalScope, storage, output)
        , init(action, globalScope)
        , parser(lexer, action)
        , generator(globalScope, order)
    {}

    bool compile(std::ostream& stream)
    {
        parser.parse();

        generator.generate(stream);
        return true;
    }
};


#define EXPECT_EQ_OR_LOG(save, test, value, output) \
                                    do                                  \
                                    {                                   \
                                        if ((test) != (value)) {        \
                                            save = true;                \
                                            output << #test << "(" << (test) << ")  != " << #value << "(" << (value) << ")\n";   \
                                        }                               \
                                    }                                   \
                                    while(false)
// EXPECT_EQ_OR_LOG set the check value to true on exit.
// So a "Successes" means that the check value remains
// false at the end of the tests.
#define EXPECT_SUCC(check, output)      EXPECT_FALSE_OR_DEBUG(check, output)

// If we expect failure.
// Then check we have failed and that we have the correct error message.
#define EXPECT_FAIL(check, error, output)   \
                                    do                                          \
                                    {                                           \
                                        bool getCheck = check;                  \
                                        EXPECT_TRUE((getCheck == true) && (output.str().find(error) != std::string::npos));    \
                                        if ((getCheck != true) || (output.str().find(error) == std::string::npos)) {           \
                                            std::cerr << "TRUE: FAIL\n";        \
                                            std::cerr << output.str() << "\n";  \
                                        }                                       \
                                    }                                           \
                                    while(false)

#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                    do                                          \
                                    {                                           \
                                        bool getCheck = check;                  \
                                        EXPECT_TRUE(getCheck);                  \
                                        if (!(getCheck)) {                      \
                                            std::cerr << "TRUE: FAIL\n";        \
                                            std::cerr << output.str();          \
                                        }                                       \
                                    }                                           \
                                    while(false)
#define EXPECT_FALSE_OR_DEBUG(check, output) \
                                    do                                          \
                                    {                                           \
                                        bool getCheck = check;                  \
                                        EXPECT_FALSE(getCheck);                 \
                                        if (getCheck) {                         \
                                            std::cerr << "FALSE FAIL\n";        \
                                            std::cerr << output.str();          \
                                        }                                       \
                                    }                                           \
                                    while(false)
#define EXPECT_THROW_OR_DEBUG(check, msg, output) \
                                    do                                          \
                                    {                                           \
                                        try {                                   \
                                            check;                              \
                                            std::cerr << "THROW FAIL (Throw expected but did not happen)\n";\
                                            std::cerr << output.str();          \
                                            EXPECT_TRUE(false);                 \
                                        }                                       \
                                        catch(std::exception const& e)          \
                                        {                                       \
                                            std::string error(e.what());        \
                                            if (error.find(msg) == std::string::npos) {\
                                                std::cerr << "THROW FAIL (Throw happened but not expected message)\n" \
                                                          << error << "\n"      \
                                                          << output.str();      \
                                                throw;                          \
                                            }                                   \
                                        }                                       \
                                        catch(std::string const& m)             \
                                        {                                       \
                                            std::cerr << "THROW FAIL (Got a string expecting a standard exception)\n" \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                        catch(int e)                            \
                                        {                                       \
                                            std::cerr << "THROW FAIL (Got an int expecting a standard exception)\n" \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                        catch(...)                              \
                                        {                                       \
                                            std::cerr << "THROW FAIL\n"         \
                                                      << "This was a non standard exception of unknown type\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                    }                                           \
                                    while(false)
#define EXPECT_NO_THROW_OR_DEBUG(check, output) \
                                    do                                          \
                                    {                                           \
                                        try                                     \
                                        {                                       \
                                            check;                              \
                                            EXPECT_TRUE(true);                  \
                                        }                                       \
                                        catch(std::exception const& e)          \
                                        {                                       \
                                            std::string error(e.what());        \
                                            std::cerr << "THROW FAIL\n"         \
                                                      << "Exception Message: " << error << "\n"  \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                        catch(std::string const& m)             \
                                        {                                       \
                                            std::cerr << "THROW FAIL\n"         \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                        catch(int e)                            \
                                        {                                       \
                                            std::cerr << "THROW FAIL\n"         \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                        catch(...)                              \
                                        {                                       \
                                            std::cerr << "THROW FAIL\n"         \
                                                      << "This was a non standard exception of unknown type\n" \
                                                      << output.str();          \
                                            throw;                              \
                                        }                                       \
                                    }                                           \
                                    while(false)

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}


#endif
