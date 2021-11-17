
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
%token              OBJECT
%token              ARRAY
%token              MAP
%token              FUNC
%token              IDENTIFIER
%token              SCOPE
%token              ARROW
%token              STRING

%%

Anvil:              NamespaceList


NamespaceList:      Namespace
                |   NamespaceList Namespace

Namespace:          NAMESPACE NameSpaceIdentifer '{' DeclListOpt '}'

DeclListOpt:
                |   DeclList
DeclList:           Decl
                |   DeclList Decl

Decl:               NAMESPACE NameSpaceIdentifer '{' DeclListOpt '}'
                |   TYPE OBJECT DeclIdentifer '{' DeclListOpt '}'
                |   TYPE ARRAY  DeclIdentifer '{' ObjectDecl '}'
                |   TYPE MAP    DeclIdentifer '{' ObjectDecl ',' ObjectDecl '}'
                |   TYPE FUNC   DeclIdentifer '(' ParamListOpt ')' ARROW ObjectDecl ';'
                |   ObjectIdentifer ':' ObjectDecl InitObject
                |   Statement

ParamListOpt:
                |   ParamList
ParamList:          Param
                |   ParamList ',' Param
Param:              ObjectIdentifer ':' ObjectDecl

ObjectDecl:         TypeName
                |   AnonDecl

AnonDecl:           OBJECT '{' DeclListOpt '}'
                |   ARRAY  '{' ObjectDecl '}'
                |   MAP    '{' ObjectDecl ',' ObjectDecl '}'
                |   FUNC   '(' ParamListOpt ')' ARROW ObjectDecl

Statement:          Expression ';'
Expression:         ExprFuncCall
ExprFuncCall:       ObjectName '(' ValueListOpt ')'


ValueListOpt:
                |   ValueList
ValueList:          Value
                |   ValueList ',' Value
Value:              ObjectName
                |   Literal

InitObject:         ';'

ObjectName:         ObjectIdentifer
                |   NameSpaceIdentifer SCOPE ObjectName
                |   ObjectIdentifer '.' ObjectName
TypeName:           DeclIdentifer
                |   NameSpaceIdentifer SCOPE TypeName
                |   ObjectIdentifer '.' TypeName


Literal:            STRING


NameSpaceIdentifer: Identifer
DeclIdentifer:      Identifer
ObjectIdentifer:    Identifer

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

