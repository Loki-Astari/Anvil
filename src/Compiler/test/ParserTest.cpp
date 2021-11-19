#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "ice.tab.hpp"
#include "test/CompileAction.h"

#include <fstream>
#include <sstream>

TEST(ParserTest, NameSpaceOnly)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameSpaceNested)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    namespace Nest
    {
    }
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameSpaceTwice)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
}

namespace Twice
{
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameStatExprFuncCall)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    Console::print("Hello World");
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeArray)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    type array TypeArray { Std::String }
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeFunc)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    type func TypeFunc(value: Std::Int) -> Std::String;
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeMap)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    type map TypeMap { Std::String, Std::Int}
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeObject)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    type object TypeObject
    {
        name:   Std::String;
        age:    Std::Int;
    }
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : array { Std::Int };
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : func(value: Std::Int, error: Std::String) -> Std::Number;
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : map { Std::Int, Std::String };
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data: object
    {
        Name:   String;
        Age:    Number;
    };
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableDecl)
{
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data: Std::String;
}
    )");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}


