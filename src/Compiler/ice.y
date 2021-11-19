
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

Anvil:              NamespaceList                                       {action.log("Anvil:              NamespaceList");}


NamespaceList:      Namespace                                           {action.log("NamespaceList:      Namespace");}
                |   NamespaceList Namespace                             {action.log("NamespaceList:      NamespaceList Namespace");}

Namespace:          NAMESPACE NameSpaceIdentifer '{' DeclListOpt '}'    {action.log("Namespace:          NAMESPACE NameSpaceIdentifer { DeclListOpt }");}

DeclListOpt:                                                            {action.log("DeclListOpt:        >EMPTY<");}
                |   DeclList                                            {action.log("DeclListOpt:        DeclList");}
DeclList:           Decl                                                {action.log("DeclList:           Decl");}
                |   DeclList Decl                                       {action.log("DeclList:           DeclList Decl");}

Decl:               NAMESPACE NameSpaceIdentifer '{' DeclListOpt '}'    {action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt }");}
                |   TYPE OBJECT TypeIdentifer '{' DeclListOpt '}'       {action.log("Decl:               TYPE OBJECT TypeIdentifer { DeclListOpt }");}
                |   TYPE ARRAY  TypeIdentifer '{' ObjectDecl '}'        {action.log("Decl:               TYPE ARRAY TypeIdentifer { ObjectDecl }");}
                |   TYPE MAP    TypeIdentifer '{' ObjectDecl ',' ObjectDecl '}'
                                                                        {action.log("Decl:               TYPE MAP TypeIdentifer { ObjectDecl , ObjectDecl }");}
                |   TYPE FUNC   TypeIdentifer '(' ParamListOpt ')'
                    ARROW ObjectDecl ';'                                {action.log("Decl:               TYPE FUNC TypeIdentifer ( ParamListOpt ) -> ObjectDecl ;");}
                |   ObjectIdentifer ':' ObjectDecl InitObject           {action.log("Decl:               ObjectIdentifer : ObjectDecl InitObject");}
                |   Statement                                           {action.log("Decl:               Statement");}

ParamListOpt:                                                           {action.log("ParamListOpt:       >EMPTY<");}
                |   ParamList                                           {action.log("ParamListOpt:       ParamList");}
ParamList:          Param                                               {action.log("ParamList:          Param");}
                |   ParamList ',' Param                                 {action.log("ParamList:          ParamList , Param");}
Param:              ObjectIdentifer ':' ObjectDecl                      {action.log("Param:              ObjectIdentifer : ObjectDecl");}

ObjectDecl:         TypeName                                            {action.log("ObjectDecl:         TypeName");}
                |   AnonDecl                                            {action.log("ObjectDecl:         AnonDecl");}

AnonDecl:           OBJECT '{' DeclListOpt '}'                          {action.log("AnonDecl:           OBJECT { DeclListOpt }");}
                |   ARRAY  '{' ObjectDecl '}'                           {action.log("AnonDecl:           ARRAY { ObjectDecl }");}
                |   MAP    '{' ObjectDecl ',' ObjectDecl '}'            {action.log("AnonDecl:           MAP { ObjectDecl , ObjectDecl }");}
                |   FUNC   '(' ParamListOpt ')' ARROW ObjectDecl        {action.log("AnonDecl:           FUNC ( ParamListOpt ) -> ObjectDecl");}

Statement:          Expression ';'                                      {action.log("Statement:          Expression ;");}
Expression:         ExprFuncCall                                        {action.log("Expression:         ExprFuncCall");}
ExprFuncCall:       ObjectName '(' ValueListOpt ')'                     {action.log("ExprFuncCall:       ObjectName ( ValueListOpt )");}


ValueListOpt:                                                           {action.log("ValueListOpt:       >EMPTY<");}
                |   ValueList                                           {action.log("ValueListOpt:       ValueList");}
ValueList:          Value                                               {action.log("ValueList:          Value");}
                |   ValueList ',' Value                                 {action.log("ValueList:          ValueList , Value");}
Value:              ObjectName                                          {action.log("Value:              ObjectName");}
                |   Literal                                             {action.log("Value:              Literal");}

InitObject:         ';'                                                 {action.log("InitObject:         ;");}

ObjectName:         ObjectIdentifer                                     {action.log("ObjectName:         ObjectIdentifer");}
                |   NameSpaceIdentifer SCOPE ObjectName                 {action.log("ObjectName:         NameSpaceIdentifer :: ObjectName");}
                |   ObjectIdentifer '.' ObjectName                      {action.log("ObjectName:         ObjectIdentifer . ObjectName");}

TypeName:           TypeNameFull                                        {action.log("TypeName:           TypeNameFull");
                                                                         $$ = action.findTypeInScope(lexer, $1);}
TypeNameFull:       TypeIdentifer                                       {action.log("TypeNameFull:       TypeIdentifer");
                                                                         $$ = action.fullIdentiferCreate(lexer); action.fullIdentiferAddPath(lexer, $$, $1);}
                |   NameSpaceIdentifer SCOPE TypeNameFull               {action.log("TypeNameFull:       NameSpaceIdentifer :: TypeNameFull");
                                                                         $$ = $3; action.fullIdentiferAddPath(lexer, $$, $1);}
                |   TypeIdentifer SCOPE TypeNameFull                    {action.log("TypeNameFull:        TypeIdentifer :: TypeNameFull");
                                                                         $$ = $3; action.fullIdentiferAddPath(lexer, $$, $1);}


Literal:            STRING


NameSpaceIdentifer: Identifer                                           {action.log("NameSpaceIdentifer: Identifer");
                                                                         $$ = action.identifierCheckNS(lexer, $1);}
TypeIdentifer:      Identifer                                           {action.log("TypeIdentifer:      Identifer");
                                                                         $$ = action.identifierCheckType(lexer, $1);}
ObjectIdentifer:    Identifer                                           {action.log("ObjectIdentifer:    Identifer");
                                                                         $$ = action.identifierCheckObject(lexer, $1);}

Identifer:          IDENTIFIER                                          {action.log("Identifer:          IDENTIFIER");
                                                                         $$ = action.identifierCreate(lexer);}



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

