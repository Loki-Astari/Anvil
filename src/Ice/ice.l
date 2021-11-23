
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

#undef YY_DECL
#define YY_DECL int Lexer::yylexWithAction(Action& action)
#define IN_LEXER
#include "ice.tab.hpp"

%}

Identifier          [A-Za-z][A-Za-z0-9_]*
WhiteSpace          [ \t\r]+
NewLine             \n
String              \"[^"]*\"

%%
{String}            {action.token();return yy::Parser::token::STRING;}

namespace           {action.token();return yy::Parser::token::NAMESPACE;}
class               {action.token();return yy::Parser::token::CLASS;}
array               {action.token();return yy::Parser::token::ARRAY;}
map                 {action.token();return yy::Parser::token::MAP;}
func                {action.token();return yy::Parser::token::FUNC;}
::                  {action.token();return yy::Parser::token::SCOPE;}
->                  {action.token();return yy::Parser::token::ARROW;}
\{                  {action.token();return '{';}
\}                  {action.token();return '}';}
\(                  {action.token();return '(';}
\)                  {action.token();return ')';}
:                   {action.token();return ':';}
;                   {action.token();return ';';}
\.                  {action.token();return '.';}
,                   {action.token();return ',';}

{Identifier}        {action.token();return yy::Parser::token::IDENTIFIER;}

{WhiteSpace}        {action.ignoreSpace();/* Ignore */}
{NewLine}           {action.newLine();    /* Ignore */}
.                   {action.invalidCharacter();}

%%

