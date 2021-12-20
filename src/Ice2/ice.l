
%option c++
%option noyywrap
%option yyclass="Lexer"

%{

#undef YY_DECL
#define YY_DECL int Lexer::yylexWithAction(Action& action)
#define IN_LEXER
#include "ice.tab.hpp"

%}


IdentifierNS        [A-Z][A-Za-z0-9_]*
IdentifierType      [A-Z][A-Za-z0-9]{3,}

WhiteSpace          [ \t\r]+
NewLine             \n

%%

namespace           {action.token();return yy::Parser::token::NAMESPACE;}
class               {action.token();return yy::Parser::token::CLASS;}
\{                  {action.token();return '{';}
\}                  {action.token();return '}';}

{IdentifierType}    {action.token();return yy::Parser::token::IDENTIFIER_TYPE;}
{IdentifierNS}      {action.token();return yy::Parser::token::IDENTIFIER_NS;}
{WhiteSpace}        {action.ignoreSpace();/* Ignore */}
{NewLine}           {action.newLine();    /* Ignore */}
.                   {action.invalidCharacter();}

%%

