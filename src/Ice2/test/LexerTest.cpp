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
array
map
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
"This is a String"
{
}
(
)
:
;
.
,
[
]
=
*=
/=
%=
+=
-=
<<=
>>=
&=
^=
|=
?
&&
||
|
^
&
!=
==
<
>
<=
>=
<<
>>
+
-
*
/
%
++
--
~
!
$
    )");

    using ThorsAnvil::Anvil::Ice::Lexer;
    using ThorsAnvil::Anvil::Ice::Namespace;
    using ThorsAnvil::Anvil::Ice::Storage;
    std::stringstream   output;

    Lexer           lexer(file);
    Namespace       globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "Global");;
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

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::LITERAL_STRING, val);
    EXPECT_EQ(lexer.lexem(), "\"This is a String\"");

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('{', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('}', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('(', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(')', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(':', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(';', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('.', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(',', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('[', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(']', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('=', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_MUL, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_DIV, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_MOD, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_ADD, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_SUB, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_L, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_R, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_AND, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_XOR, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_ASSIGN_OR, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('?', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_AND, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_OR, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('|', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('^', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('&', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_NE, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_EQ, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('<', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('>', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_LE, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_GE, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_L_SHIFT, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_R_SHIFT, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('+', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('-', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('*', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('/', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('%', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_INC, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ(yy::Parser::token::OP_DEC, val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('~', val);

    val = lexer.yylexWithActionGo(action);
    EXPECT_EQ('!', val);

    EXPECT_THROW_OR_DEBUG(val = lexer.yylexWithActionGo(action), "Invalid Character", output);

}



