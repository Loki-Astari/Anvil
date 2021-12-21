#include <gtest/gtest.h>

#include "Lexer.h"
#include "Storage.h"
#include "Declaration.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>

TEST(LexerTest, GetLexemeFromNotStartedLexer)
{
    std::stringstream   file = buildStream(R"()");
    Lexer               lexer(file);

    lexer.lexem();
}

TEST(LexerTest, GetLexemeEmptyStream)
{
    std::stringstream   file = buildStream("");
    Lexer               lexer(file);

    lexer.lexem();
    lexer.lexem();
    lexer.lexem();
}

TEST(LexerTest, ReadSymbolsFile)
{
#if 0
-namespace
-class
array
map
-func
-objectName
-TypeName
-Ns
-Name_Space
-::
-->
"This is a String"
-{
-}
(
)
-:
-;
.
-,
#endif
    std::stringstream    file = buildStream(R"(
namespace
class
func
objectName
TypeName
Ns
Name_Space
::
->
{
}
:
;
,
$
    )");

    using ThorsAnvil::Anvil::Ice::Lexer;
    using ThorsAnvil::Anvil::Ice::Namespace;
    using ThorsAnvil::Anvil::Ice::Storage;
    std::stringstream   output;

    Lexer           lexer(file);
    Namespace       globalScope("Global");;
    Storage         storage;
    Action          action(lexer, globalScope, storage, output);

    int val;


    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::NAMESPACE, val);
    EXPECT_EQ(lexer.lexem(), "namespace");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::CLASS, val);
    EXPECT_EQ(lexer.lexem(), "class");

#if 0
    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::ARRAY, val);
    EXPECT_EQ(lexer.lexem(), "array");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::MAP, val);
    EXPECT_EQ(lexer.lexem(), "map");
#endif

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::FUNC, val);
    EXPECT_EQ(lexer.lexem(), "func");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::IDENTIFIER_OBJECT, val);
    EXPECT_EQ(lexer.lexem(), "objectName");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::IDENTIFIER_TYPE, val);
    EXPECT_EQ(lexer.lexem(), "TypeName");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::IDENTIFIER_NS, val);
    EXPECT_EQ(lexer.lexem(), "Ns");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::IDENTIFIER_NS, val);
    EXPECT_EQ(lexer.lexem(), "Name_Space");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::SCOPE, val);
    EXPECT_EQ(lexer.lexem(), "::");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::ARROW, val);
    EXPECT_EQ(lexer.lexem(), "->");

#if 0
    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::STRING, val);
    EXPECT_EQ(lexer.lexem(), "\"This is a String\"");
#endif

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('{', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('}', val);

#if 0
    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('(', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(')', val);
#endif

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(':', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(';', val);

#if 0
    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('.', val);
#endif

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(',', val);

    EXPECT_THROW_OR_DEBUG(val = lexer.yylexWithActionGo(action), "Invalid Character", output);

}



