
%skeleton "lalr1.cc"
%require "2.1a"
%defines
%define "parser_class_name" "Parser"

%{

#include "Lexer.h"
#include "Action.h"

using ThorsAnvil::Anvil::Ice::Lexer;
using ThorsAnvil::Anvil::Ice::Action;


//namespace ThorsAnvial::Anvil::Ice {
int yylex(void*, ThorsAnvil::Anvil::Ice::Lexer& lexer, ThorsAnvil::Anvil::Ice::Action& action);
//}

#define YYSTYPE ThorsAnvil::Anvil::Ice::Int
//#define YYDEBUG 1

%}


%parse-param {Lexer&  lexer}
%parse-param {Action& action}
%lex-param   {Lexer&  lexer}
%lex-param   {Action& action}


%token              NAMESPACE
%token              TYPE
%token              IDENTIFIER
%token              SCOPE

%%

Anvil:              NamespaceList


NamespaceList:      Namespace
                |   NamespaceList Namespace

Namespace:          NAMESPACE NameSpaceIdentifer '{' TypeList '}'

TypeList:           Type
                |   TypeList Type

Type:               TYPE TypeIdentifer '{' ObjectList '}'

ObjectList:         Object
                |   ObjectList Object

Object:             ObjectIdentifer ':' Type ';'

NameSpaceIdentifer: Identifer
TypeIdentifer:      Identifer
ObjectIdentifer:    Identifer

Type:               Identifer
                |   Type SCOPE Identifer

Identifer:          IDENTIFIER



%%

#include <iostream>
#include <sstream>

int yylex(void*, Lexer& lexer, Action& action)
{
    (void)action;
    return lexer.yylex();
}

void yy::Parser::error(yy::location const&, std::string const& msg)
{
    std::string lastToken(lexer.YYText(), lexer.YYText() + lexer.YYLeng());
    std::stringstream extended;
    extended << "yy::Parser::error: Error: " << msg << " -> Last Token: " << lastToken << " At line: " << lexer.lineno();
    throw std::runtime_error(extended.str());
}

