#include <gtest/gtest.h>

#include "Lexer.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>

TEST(LexerTest, ReadSymbolsFile)
{
    std::stringstream    file = buildStream(R"(
namespace
class
array
map
func
Bob
::
->
"This is a String"
{
}
(
)
:
;
.
,
$
    )");

    using ThorsAnvil::Anvil::Ice::Lexer;
    std::stringstream   output;

    Action          action(output);
    Lexer           lexer(action, file);

    int val;


    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::NAMESPACE, val);
    EXPECT_EQ(lexer.lexem(), "namespace");

    val = lexer.yylex();
    EXPECT_EQ(yy::Parser::token::CLASS, val);
    EXPECT_EQ(lexer.lexem(), "class");

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

    EXPECT_THROW_OR_DEBUG(val = lexer.yylex(), "Invalid Character", output);

}


