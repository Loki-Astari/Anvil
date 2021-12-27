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

TEST(ConstructorTest, Default)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorTest, UserDefined)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer
    {
        construct{}{}
    }
    class String
    {
        construct{}{}
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorTest, UserDefinedNotDefault)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer
    {
        construct{Integer}{}
    }
    class String
    {
        construct{String}{}
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorTest, UserDefinedWithMembers)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
}
namespace My_Namespace
{
    class MyClass
    {
        a1 : Std::Integer;
        a2 : Std::Integer;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorTest, UserDefinedWithMembersNoDefault)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class String {}
    class Integer
    {
        construct {}{}
        construct {String}{}
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
