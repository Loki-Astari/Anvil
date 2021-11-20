
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

// #undef YY_DECL
// #define YY_DECL int Lexer::doLex(Action&)
#define IN_LEXER
#include "ice.tab.hpp"

%}

Identifier          [A-Za-z][A-Za-z0-9_]*
WhiteSpace          [ \t\r]+
NewLine             \n
String              \"[^"]*\"

%%
{String}            {action.token(*this);return yy::Parser::token::STRING;}

namespace           {action.token(*this);return yy::Parser::token::NAMESPACE;}
type                {action.token(*this);return yy::Parser::token::TYPE;}
object              {action.token(*this);return yy::Parser::token::OBJECT;}
array               {action.token(*this);return yy::Parser::token::ARRAY;}
map                 {action.token(*this);return yy::Parser::token::MAP;}
func                {action.token(*this);return yy::Parser::token::FUNC;}
::                  {action.token(*this);return yy::Parser::token::SCOPE;}
->                  {action.token(*this);return yy::Parser::token::ARROW;}
\{                  {action.token(*this);return '{';}
\}                  {action.token(*this);return '}';}
\(                  {action.token(*this);return '(';}
\)                  {action.token(*this);return ')';}
:                   {action.token(*this);return ':';}
;                   {action.token(*this);return ';';}
\.                  {action.token(*this);return '.';}
,                   {action.token(*this);return ',';}

{Identifier}        {action.token(*this);return yy::Parser::token::IDENTIFIER;}

{WhiteSpace}        {action.ignoreSpace(*this);/* Ignore */}
{NewLine}           {action.newLine(*this);    /* Ignore */}
.                   {action.invalidCharacter(*this);}

%%

