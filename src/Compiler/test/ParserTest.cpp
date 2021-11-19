#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "ice.tab.hpp"
#include "test/CompileAction.h"

#include <fstream>

#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_TRUE(check);         \
                                        } catch(...) {                  \
                                            output.display(std::cout);  \
                                            throw;                      \
                                        }                               \
                                      }

TEST(ParserTest, NameSpaceOnly)
{
    std::ifstream   file("test/data/NameSpaceOnly.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameSpaceNested)
{
    std::ifstream   file("test/data/NameSpaceNested.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameSpaceTwice)
{
    std::ifstream   file("test/data/NameSpaceTwice.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameStatExprFuncCall)
{
    std::ifstream   file("test/data/NameStatExprFuncCall.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeArray)
{
    std::ifstream   file("test/data/NameTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeFunc)
{
    std::ifstream   file("test/data/NameTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeMap)
{
    std::ifstream   file("test/data/NameTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameTypeObject)
{
    std::ifstream   file("test/data/NameTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::ifstream   file("test/data/NameVariableAnonTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::ifstream   file("test/data/NameVariableAnonTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::ifstream   file("test/data/NameVariableAnonTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::ifstream   file("test/data/NameVariableAnonTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}

TEST(ParserTest,NameVariableDecl)
{
    std::ifstream   file("test/data/NameVariableDecl.ice");

    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE_OR_DEBUG(parser.parse(), action);
}


