
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

#undef YY_DECL
#define YY_DECL int Lexer::yylexWithAction(Action& action)
#define IN_LEXER
#include "ice.tab.hpp"

%}

IdentifierObject    [a-z][A-Za-z0-9_]*
IdentifierNS        [A-Z][A-Za-z0-9_]*
IdentifierType      [A-Z][A-Za-z0-9]{3,}
LiteralString       \"[^"]*\"

WhiteSpace          [ \t\r]+
NewLine             \n

%%

namespace           {action.token();return yy::Parser::token::NAMESPACE;}
class               {action.token();return yy::Parser::token::CLASS;}
func                {action.token();return yy::Parser::token::FUNC;}
->                  {action.token();return yy::Parser::token::ARROW;}
::                  {action.token();return yy::Parser::token::SCOPE;}
\{                  {action.token();return '{';}
\}                  {action.token();return '}';}
\(                  {action.token();return '(';}
\)                  {action.token();return ')';}
\[                  {action.token();return '[';}
\]                  {action.token();return ']';}
:                   {action.token();return ':';}
;                   {action.token();return ';';}
,                   {action.token();return ',';}
=                   {action.token();return '=';}
\*=                 {action.token();return yy::Parser::token::OP_ASSIGN_MUL;}
\/=                 {action.token();return yy::Parser::token::OP_ASSIGN_DIV;}
\%=                 {action.token();return yy::Parser::token::OP_ASSIGN_MOD;}
\+=                 {action.token();return yy::Parser::token::OP_ASSIGN_ADD;}
\-=                 {action.token();return yy::Parser::token::OP_ASSIGN_SUB;}
\<\<=               {action.token();return yy::Parser::token::OP_ASSIGN_L;}
\>\>=               {action.token();return yy::Parser::token::OP_ASSIGN_R;}
\&=                 {action.token();return yy::Parser::token::OP_ASSIGN_AND;}
\^=                 {action.token();return yy::Parser::token::OP_ASSIGN_XOR;}
\|=                 {action.token();return yy::Parser::token::OP_ASSIGN_OR;}
\?                  {action.token();return '?';}
\&\&                {action.token();return yy::Parser::token::OP_AND;}
\|\|                {action.token();return yy::Parser::token::OP_OR;}
\|                  {action.token();return '|';}
\^                  {action.token();return '^';}
\&                  {action.token();return '&';}
\!=                 {action.token();return yy::Parser::token::OP_NE;}
==                  {action.token();return yy::Parser::token::OP_EQ;}
\<                  {action.token();return '<';}
\>                  {action.token();return '>';}
\<=                 {action.token();return yy::Parser::token::OP_LE;}
\>=                 {action.token();return yy::Parser::token::OP_GE;}
\<\<                {action.token();return yy::Parser::token::OP_L_SHIFT;}
>>                  {action.token();return yy::Parser::token::OP_R_SHIFT;}
\+                  {action.token();return '+';}
\-                  {action.token();return '-';}
\*                  {action.token();return '*';}
\/                  {action.token();return '/';}
%                   {action.token();return '%';}
\+\+                {action.token();return yy::Parser::token::OP_INC;}
\-\-                {action.token();return yy::Parser::token::OP_DEC;}
\~                  {action.token();return '~';}
\!                  {action.token();return '!';}
\.                  {action.token();return '.';}


{LiteralString}     {action.token();return yy::Parser::token::LITERAL_STRING;}


{IdentifierObject}  {action.token();return yy::Parser::token::IDENTIFIER_OBJECT;}
{IdentifierType}    {action.token();return yy::Parser::token::IDENTIFIER_TYPE;}
{IdentifierNS}      {action.token();return yy::Parser::token::IDENTIFIER_NS;}
{WhiteSpace}        {action.ignoreSpace();/* Ignore */}
{NewLine}           {action.newLine();    /* Ignore */}
.                   {action.invalidCharacter();}

%%

