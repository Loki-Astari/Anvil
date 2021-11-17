
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

// #define YY_DECL int yyFlexLexer::yylex()
#define IN_LEXER
#include "ice.tab.hpp"

%}

Identifier          [A-Za-z][A-Za-z0-9]*
WhiteSpace          [ \t\r\n]+
String              \"[^"]*\"

%%
{String}            {return yy::Parser::token::STRING;}

namespace           {return yy::Parser::token::NAMESPACE;}
type                {return yy::Parser::token::TYPE;}
object              {return yy::Parser::token::OBJECT;}
array               {return yy::Parser::token::ARRAY;}
map                 {return yy::Parser::token::MAP;}
func                {return yy::Parser::token::FUNC;}
::                  {return yy::Parser::token::SCOPE;}
->                  {return yy::Parser::token::ARROW;}
\{                  {return '{';}
\}                  {return '}';}
\(                  {return '(';}
\)                  {return ')';}
:                   {return ':';}
;                   {return ';';}
\.                  {return '.';}
,                   {return ',';}

{Identifier}        {return yy::Parser::token::IDENTIFIER;}

{WhiteSpace}        {/* Ignore */}

%%

