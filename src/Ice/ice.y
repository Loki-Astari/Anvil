
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
%token              CLASS
%token              ARRAY
%token              MAP
%token              FUNC
%token              IDENTIFIER_OBJECT
%token              IDENTIFIER_TYPE
%token              IDENTIFIER_NS
%token              SCOPE
%token              ARROW
%token              STRING

%%

Anvil:                                                                  { /* Empty File */ $$ = 0;}
                |   NamespaceList                                       {
                                                                            action.log("Anvil:              NamespaceList");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }


NamespaceList:      Namespace                                           {
                                                                            action.log("NamespaceList:      Namespace");
                                                                            action.assertNoStorage($1);
                                                                        }
                |   NamespaceList Namespace                             {
                                                                            action.log("NamespaceList:      NamespaceList Namespace");
                                                                            action.assertNoStorage($1);
                                                                            action.assertNoStorage($2);
                                                                        }

Namespace:          NAMESPACE NotNameSpaceIdentifier                    {error(yy::location{}, "Invalid Identifier for Namespace");}
                |   NAMESPACE NameSpaceIdentifier '{'                   {
                                                                            action.log("Namespace:          NAMESPACE NameSpaceIdentifier { DeclListOpt } P1A");
                                                                            $$ = action.scopeAddNamespace($2);
                                                                        }
                                                     DeclListOpt '}'    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifier { DeclListOpt } P2A");
                                                                            action.assertNoStorage(action.scopeClose($4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }

DeclListOpt:                                                            {
                                                                            action.log("DeclListOpt:        >EMPTY<");
                                                                            $$ = 0;
                                                                        }
                |   DeclList                                            {
                                                                            action.log("DeclListOpt:        DeclList");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }
DeclList:           Decl                                                {
                                                                            action.log("DeclList:           Decl");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }
                |   DeclList Decl                                       {
                                                                            action.log("DeclList:           DeclList Decl");
                                                                            action.assertNoStorage($1);
                                                                            action.assertNoStorage($2);
                                                                            $$ = 0;
                                                                        }

Decl:               Namespace                                           {
                                                                            action.log("Decl:               Namespace");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }
                |   CLASS   NotTypeIdentifier                           {error(yy::location{}, "Invalid Identifier for Type");}
                |   CLASS   TypeIdentifier '{'                          {
                                                                            action.log("Decl:               CLASS TypeIdentifier { DeclListOpt } P1");
                                                                            $$ = action.scopeAddClass($2);
                                                                        }
                                             DeclListOpt '}'            {
                                                                            action.log("Decl:               CLASS TypeIdentifier { DeclListOpt } P2");
                                                                            action.releaseStorage(action.scopeClose($4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }
                |   ARRAY  NotTypeIdentifier                            {error(yy::location{}, "Invalid Identifier for Type");}
                |   ARRAY  TypeIdentifier '{' ObjectDecl '}'            {
                                                                            action.log("Decl:               ARRAY TypeIdentifier { ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddArray($2, $4));
                                                                            $$ = 0;
                                                                        }
                |   MAP    NotTypeIdentifier                            {error(yy::location{}, "Invalid Identifier for Type");}
                |   MAP    TypeIdentifier '{' ObjectDecl ',' ObjectDecl '}' {
                                                                            action.log("Decl:               MAP TypeIdentifier { ObjectDecl , ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddMap($2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   FUNC   NotTypeIdentifier                            {error(yy::location{}, "Invalid Identifier for Type");}
                |   FUNC   TypeIdentifier '{' ParamListOpt ARROW ObjectDecl '}' {
                                                                            action.log("Decl:               FUNC TypeIdentifier ( ParamListOpt ) -> ObjectDecl ;");
                                                                            action.releaseStorage(action.scopeAddFunc($2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   NotObjectIdentifier                                 {error(yy::location{}, "Invalid Identifier for Object");}
                |   ObjectIdentifier ':' ObjectDecl InitObject          {
                                                                            action.log("Decl:               ObjectIdentifier : ObjectDecl InitObject ;");
                                                                            // TODO Use InitObject
                                                                            action.assertNoStorage(action.scopeAddObject($1, $3, $4));
                                                                            $$ = 0;
                                                                        }
                |   Statement                                           {
                                                                            action.log("Decl:               Statement");
                                                                            action.assertNoStorage(action.scopeAddStatement($1));
                                                                            $$ = 0;
                                                                        }

ParamListOpt:                                                           {
                                                                            action.log("ParamListOpt:       >EMPTY<");
                                                                            $$ = action.paramListCreate();
                                                                        }
                |   ParamList                                           {
                                                                            action.log("ParamListOpt:       ParamList");
                                                                            $$ = $1;
                                                                        }
ParamList:          ObjectDecl                                          {
                                                                            action.log("ParamList:          ObjectDecl");
                                                                            $$ = action.paramListAdd(action.paramListCreate(), $1);
                                                                        }
                |   ParamList ',' ObjectDecl                            {
                                                                            action.log("ParamList:          ParamList , ObjectDecl");
                                                                            $$ = action.paramListAdd($1, $3);
                                                                        }

ObjectDecl:         TypeName                                            {
                                                                            action.log("ObjectDecl:         TypeName");
                                                                            $$ = $1;
                                                                        }
                |   AnonDecl                                            {
                                                                            action.log("ObjectDecl:         AnonDecl");
                                                                            $$ = $1;
                                                                        }

AnonDecl:           CLASS  '{'                                          {
                                                                            action.log("AnonDecl:           CLASS { DeclListOpt } P1");
                                                                            $$ = action.scopeAddClass(action.generateAnonName());
                                                                        }
                               DeclListOpt '}'                          {
                                                                            action.log("AnonDecl:           CLASS { DeclListOpt } P2");
                                                                            $$ = action.scopeClose($3);
                                                                            action.assertNoStorage($4);
                                                                        }
                |   ARRAY  '{' ObjectDecl '}'                           {
                                                                            action.log("AnonDecl:           ARRAY { ObjectDecl }");
                                                                            $$ = action.scopeAddArray(action.generateAnonName(), $3);
                                                                        }
                |   MAP    '{' ObjectDecl ',' ObjectDecl '}'            {
                                                                            action.log("AnonDecl:           MAP { ObjectDecl , ObjectDecl }");
                                                                            $$ = action.scopeAddMap(action.generateAnonName(), $3, $5);
                                                                        }
                |   FUNC   '{' ParamListOpt ARROW ObjectDecl '}'        {
                                                                            action.log("AnonDecl:           FUNC { ParamListOpt -> ObjectDecl }");
                                                                            $$ = action.scopeAddFunc(action.generateAnonName(), $3, $5);
                                                                        }

Statement:          Expression ';'                                      {
                                                                            action.log("Statement:          Expression ;");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }
Expression:         ExprFuncCall                                        {
                                                                            action.log("Expression:         ExprFuncCall");
                                                                            action.assertNoStorage($1);
                                                                            $$ = 0;
                                                                        }
ExprFuncCall:       Value '(' ValueListOpt ')'                          {
                                                                            action.log("ExprFuncCall:       Value ( ValueListOpt )");
                                                                            action.assertNoStorage(action.codeAddFunctionCall($1, $3));
                                                                            $$ = 0;
                                                                        }


ValueListOpt:                                                           {
                                                                            action.log("ValueListOpt:       >EMPTY<");
                                                                            $$ = action.objectListCreate();
                                                                        }
                |   ValueList                                           {
                                                                            action.log("ValueListOpt:       ValueList");
                                                                            $$ = $1;
                                                                        }
ValueList:          Value                                               {
                                                                            action.log("ValueList:          Value");
                                                                            $$ = action.objectListAdd(action.objectListCreate(), $1);
                                                                        }
                |   ValueList ',' Value                                 {
                                                                            action.log("ValueList:          ValueList , Value");
                                                                            $$ = action.objectListAdd($1, $2);
                                                                        }
MapListOpt:
                |   MapList
MapList:            MapValue
                |   MapList ',' MapValue
MapValue:           Value ARROW Value

Value:              ObjectName                                          {
                                                                            action.log("Value:              ObjectName");
                                                                            $$ = $1;
                                                                        }
                |   Literal                                             {
                                                                            action.log("Value:              Literal");
                                                                            $$ = $1;
                                                                        }

InitObject:         ';'                                                 {action.log("InitObject:         >EMPTY<");             /* TODO */$$ = 0;}
                |   '=' '(' ValueListOpt ')' ';'                        {action.log("InitObject:         = ( ValueListOpt )");  /* TODO */$$ = $3;}
                |   '=' '[' ValueListOpt ']' ';'                        {action.log("InitObject:         = [ ValueListOpt ]");  /* TODO */$$ = $3;}
                |   '=' '{' MapListOpt   '}' ';'                        {action.log("InitObject:         = { MapListOpt   }");  /* TODO */$$ = $3;}
                |   '{'                                                 {
                                                                            action.log("InitObject:         { DeclListOpt } P1");
                                                                            $$ = action.scopeAddCodeBlock();
                                                                        }
                        DeclListOpt '}'                                 {
                                                                            action.log("InitObject:         { DeclListOpt } P2");
                                                                            action.assertNoStorage(action.scopeClose($2));
                                                                            action.assertNoStorage($3);
                                                                            $$ = 0;
                                                                        }

ObjectName:         ObjectNameFull                                      {
                                                                            action.log("ObjectName:         ObjectNameFull");
                                                                            $$ = $1;
                                                                        }


ObjectNameFull:     ObjectPath                                          {
                                                                            action.log("ObjectNameFull:     ObjectPath");
                                                                            $$ = action.objectIDCreate(action.identifierListCreate(), $1);
                                                                        }
                |   ObjectScope ObjectPath                              {
                                                                            action.log("ObjectNameFull:     ObjectScope SCOPE ObjectPath");
                                                                            $$ = action.objectIDCreate($1, $2);
                                                                        }


ObjectScope:        NameSpaceIdentifier SCOPE                           {
                                                                            action.log("ObjectScope:        NameSpaceIdentifier");
                                                                            $$ = action.identifierListAdd(action.identifierListCreate(), $1);
                                                                        }
                |   NameSpaceIdentifier SCOPE ObjectScope               {
                                                                            action.log("ObjectScope:        ObjectScope SCOPE NameSpaceIdentifier");
                                                                            $$ = action.identifierListAdd($3, $1);
                                                                        }

ObjectPath:         ObjectIdentifier                                    {
                                                                            action.log("ObjectPath:         ObjectIdentifier");
                                                                            $$ = action.identifierListAdd(action.identifierListCreate(), $1);
                                                                        }
                |   ObjectIdentifier '.' ObjectPath                     {
                                                                            action.log("ObjectPath:         ObjectPath '.' ObjectIdentifier");
                                                                            $$ = action.identifierListAdd($3, $1);
                                                                        }

TypeName:           TypeNameFull                                        {
                                                                            action.log("TypeName:           TypeNameFull");
                                                                            $$ = action.findTypeInScope($1);
                                                                        }
TypeNameFull:       TypeIdentifier                                      {
                                                                            action.log("TypeNameFull:       TypeIdentifier");
                                                                            $$ = action.identifierListAdd(action.identifierListCreate(), $1);
                                                                        }
                |   NameSpaceIdentifier SCOPE TypeNameFull              {
                                                                            action.log("TypeNameFull:       NameSpaceIdentifier :: TypeNameFull");
                                                                            $$ = action.identifierListAdd($3, $1);
                                                                        }
                |   TypeIdentifier SCOPE TypeNameFull                   {
                                                                            action.log("TypeNameFull:        TypeIdentifier :: TypeNameFull");
                                                                            $$ = action.identifierListAdd($3, $1);
                                                                        }


Literal:            STRING                                              {
                                                                            action.log("Literal:            STRING");
                                                                            $$ = action.addLiteralString();
                                                                        }

NameSpaceIdentifier:IDENTIFIER_NS                                       {
                                                                            action.log("NameSpaceIdentifier:IDENTIFIER_NS");
                                                                            $$ = action.identifierCheckNS(action.identifierCreate(lexer.lexem()));
                                                                        }
TypeIdentifier:     IDENTIFIER_TYPE                                     {
                                                                            action.log("TypeIdentifier:     IDENTIFIER_TYPE");
                                                                            $$ = action.identifierCheckType(action.identifierCreate(lexer.lexem()));
                                                                        }
ObjectIdentifier:   IDENTIFIER_OBJECT                                   {
                                                                            action.log("ObjectIdentifier:   IDENTIFIER_OBJECT");
                                                                            $$ = action.identifierCheckObject(action.identifierCreate(lexer.lexem()));
                                                                        }

NotNameSpaceIdentifier:     TypeIdentifier
                        |   ObjectIdentifier
NotTypeIdentifier:          NameSpaceIdentifier
                        |   ObjectIdentifier

NotObjectIdentifier:        NameSpaceIdentifier
                        |   TypeIdentifier



%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithAction(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

