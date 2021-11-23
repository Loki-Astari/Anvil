#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

struct ParserCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    ParserCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , action(lexer, output)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(ParserTest, NameSpaceOnly)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameSpaceNested)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    namespace Nest
    {
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameSpaceTwice)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
}

namespace Twice
{
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameStatExprFuncCall)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    Console::print("Hello World");
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeArray)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    array TypeArray { Std::String }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    func TypeFunc{Std::Integer -> Std::String}
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeMap)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    map TypeMap { Std::String, Std::Integer}
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    class ClassName
    {
        name:   Std::String;
        age:    Std::Integer;
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : array { Std::Integer };
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : func{Std::Integer, Std::String -> Std::Number};
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : map { Std::Integer, Std::String };
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data: class
    {
        Name:   String;
        Age:    Number;
    };
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableDecl)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data: Std::String;
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}


