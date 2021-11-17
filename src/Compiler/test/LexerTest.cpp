#include <gtest/gtest.h>

#include "Lexer.h"
#include "Action.h"
#include "ice.tab.hpp"

#include <fstream>

TEST(LexerTest, ReadSymbolsFile)
{
    std::ifstream   file("test/data/lexerTestSymbols");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Lexer;

    CompileAction   action;
    Lexer           lexer(action, file);

    int val;


    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::NAMESPACE, val);
    EXPECT_EQ(lexer.lexem(), "namespace");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::TYPE, val);
    EXPECT_EQ(lexer.lexem(), "type");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::OBJECT, val);
    EXPECT_EQ(lexer.lexem(), "object");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::ARRAY, val);
    EXPECT_EQ(lexer.lexem(), "array");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::MAP, val);
    EXPECT_EQ(lexer.lexem(), "map");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::FUNC, val);
    EXPECT_EQ(lexer.lexem(), "func");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::IDENTIFIER, val);
    EXPECT_EQ(lexer.lexem(), "Bob");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::SCOPE, val);
    EXPECT_EQ(lexer.lexem(), "::");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::ARROW, val);
    EXPECT_EQ(lexer.lexem(), "->");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::STRING, val);
    EXPECT_EQ(lexer.lexem(), "\"This is a String\"");

    val = lexer.yylex();
    EXPECT_EQ('{', val);

    val = lexer.yylex();
    EXPECT_EQ('}', val);

    val = lexer.yylex();
    EXPECT_EQ('(', val);

    val = lexer.yylex();
    EXPECT_EQ(')', val);

    val = lexer.yylex();
    EXPECT_EQ(':', val);

    val = lexer.yylex();
    EXPECT_EQ(';', val);

    val = lexer.yylex();
    EXPECT_EQ('.', val);

    val = lexer.yylex();
    EXPECT_EQ(',', val);
}



