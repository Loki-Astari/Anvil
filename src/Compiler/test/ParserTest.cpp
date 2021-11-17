#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Action.h"
#include "ice.tab.hpp"

#include <fstream>

TEST(ParserTest, NameSpaceOnly)
{
    std::ifstream   file("test/data/NameSpaceOnly.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameSpaceNested)
{
    std::ifstream   file("test/data/NameSpaceNested.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameSpaceTwice)
{
    std::ifstream   file("test/data/NameSpaceTwice.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameStatExprFuncCall)
{
    std::ifstream   file("test/data/NameStatExprFuncCall.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameTypeArray)
{
    std::ifstream   file("test/data/NameTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameTypeFunc)
{
    std::ifstream   file("test/data/NameTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameTypeMap)
{
    std::ifstream   file("test/data/NameTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameTypeObject)
{
    std::ifstream   file("test/data/NameTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::ifstream   file("test/data/NameVariableAnonTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::ifstream   file("test/data/NameVariableAnonTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::ifstream   file("test/data/NameVariableAnonTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::ifstream   file("test/data/NameVariableAnonTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest,NameVariableDecl)
{
    std::ifstream   file("test/data/NameVariableDecl.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}


