#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

TEST(ParserTest, NameSpaceOnly)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
}
    )");

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
}

TEST(ParserTest,NameTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    func TypeFunc{value: Std::Integer -> Std::String}
}
    )");

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
}

TEST(ParserTest,NameTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    type TypeObject
    {
        name:   Std::String;
        age:    Std::Integer;
    }
}
    )");

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data : func{value: Std::Integer, error: Std::String -> Std::Number};
}
    )");

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name
{
    data: type
    {
        Name:   String;
        Age:    Number;
    };
}
    )");

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
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

    using ThorsAnvil::Anvil::Ice::Action;
    using ThorsAnvil::Anvil::Ice::Parser;

    Action          action(result);
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), result);
}


