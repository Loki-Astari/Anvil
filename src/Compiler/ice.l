
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

// #define YY_DECL int yyFlexLexer::yylex()
#define IN_LEXER
#include "ice.tab.hpp"

%}

Identifier          [A-Za-z][A-Za-z0-9]*
Space               [ \t\r\n]+

%%
namespace           {return yy::Parser::token::NAMESPACE;}
{Identifier}        {return yy::Parser::token::IDENTIFIER;}
::                  {return yy::Parser::token::SCOPE;}
\{                  {return '{';}
\}                  {return '}';}
:                   {return ':';}
;                   {return ';';}

{Space}             {/* Ignore */}

%%

