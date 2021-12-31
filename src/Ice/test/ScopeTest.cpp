#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Storage.h"
#include "Declaration.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

struct FacadeCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Namespace       globalScope;
    ThorsAnvil::Anvil::Ice::Storage         storage;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    FacadeCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, globalScope, storage, output)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(ScopeTest, OneObjectDataInScope)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : Void;
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ScopeTest, OneObjectDataInScopeTwice)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : Void;
    data : Void;
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Object Already defined:", result);
}

TEST(ScopeTest, OneFunctionDataInScope)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ScopeTest, OneFunctionDataInScopeTwiceDifferentParam)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
    data : func { Void, Void -> Void}{}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
TEST(ScopeTest, OneFunctionDataInScopeTwiceSameParam)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
    data : func { Void -> Void}{}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Function Already defined:", result);
}
