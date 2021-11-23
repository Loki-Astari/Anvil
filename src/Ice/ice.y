
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
%token              IDENTIFIER
%token              SCOPE
%token              ARROW
%token              STRING

%%

Anvil:              NamespaceList                                       {
                                                                            action.log("Anvil:              NamespaceList");$$ = 0;YYACCEPT;
                                                                            action.assertNoStorage($1);
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

Namespace:          NAMESPACE NameSpaceIdentifer '{'                    {
                                                                            action.log("Namespace:          NAMESPACE NameSpaceIdentifer { DeclListOpt } P1A");
                                                                            $$ = action.scopeAddNamespace($2);
                                                                        }
                                                     DeclListOpt '}'    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P2A");
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

Decl:               NAMESPACE NameSpaceIdentifer '{'                    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P1B");
                                                                            $$ = action.scopeAddNamespace($2);
                                                                        }
                                                     DeclListOpt '}'    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P2B");
                                                                            action.assertNoStorage(action.scopeClose($4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }

                |   CLASS   TypeIdentifer '{'                           {
                                                                            action.log("Decl:               CLASS TypeIdentifer { DeclListOpt } P1");
                                                                            $$ = action.scopeAddClass($2);
                                                                        }
                                             DeclListOpt '}'            {
                                                                            action.log("Decl:               CLASS TypeIdentifer { DeclListOpt } P2");
                                                                            action.releaseStorage(action.scopeClose($4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }
                |   ARRAY  TypeIdentifer '{' ObjectDecl '}'             {
                                                                            action.log("Decl:               ARRAY TypeIdentifer { ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddArray($2, $4));
                                                                            $$ = 0;
                                                                        }
                |   MAP    TypeIdentifer '{' ObjectDecl ',' ObjectDecl '}' {
                                                                            action.log("Decl:               MAP TypeIdentifer { ObjectDecl , ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddMap($2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   FUNC   TypeIdentifer '{' ParamListOpt ARROW ObjectDecl '}' {
                                                                            action.log("Decl:               FUNC TypeIdentifer ( ParamListOpt ) -> ObjectDecl ;");
                                                                            action.releaseStorage(action.scopeAddFunc($2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   ObjectIdentifer ':' ObjectDecl InitObject           {
                                                                            action.log("Decl:               ObjectIdentifer : ObjectDecl InitObject");
                                                                            // TODO Use InitObject
                                                                            action.assertNoStorage(action.scopeAddObject($1, $3));
                                                                            $$ = 0;
                                                                        }
                |   Statement                                           {
                                                                            action.log("Decl:               Statement");
                                                                            // TODO
                                                                            // action.releaseStorage(action.scopeAddStatement($1));
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

Statement:          Expression ';'                                      {action.log("Statement:          Expression ;");}
Expression:         ExprFuncCall                                        {action.log("Expression:         ExprFuncCall");}
ExprFuncCall:       ObjectName '(' ValueListOpt ')'                     {
                                                                            action.log("ExprFuncCall:       ObjectName ( ValueListOpt )");
                                                                            $$ = action.createFuncCall($1, $3);
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
Value:              ObjectName                                          {
                                                                            action.log("Value:              ObjectName");
                                                                            $$ = $1;
                                                                        }
                |   Literal                                             {
                                                                            action.log("Value:              Literal");
                                                                            $$ = $1;
                                                                        }

InitObject:         ';'                                                 {action.log("InitObject:         ;");}

ObjectName:         ObjectNameFull                                      {
                                                                            action.log("ObjectName:         ObjectNameFull");
                                                                            $$ = action.findObjectInScope($1);
                                                                        }


ObjectNameFull:     ObjectNamePart                                      {
                                                                            action.log("ObjectNameFull:     ObjectNamePart");
                                                                            $$ = $1;
                                                                        }
                |   NameSpaceIdentifer SCOPE ObjectNameFull             {
                                                                            action.log("ObjectNameFull:     NameSpaceIdentifer :: ObjectName");
                                                                            $$ = action.fullIdentiferAddPath($3, $1);
                                                                        }
                |   TypeIdentifer SCOPE ObjectNameFull                  {
                                                                            action.log("ObjectNameFull:     TypeIdentifer :: ObjectName");
                                                                            $$ = action.fullIdentiferAddPath($3, $1);
                                                                        }
ObjectNamePart:     ObjectIdentifer                                     {
                                                                            action.log("ObjectNamePart:     ObjectIdentifer");
                                                                            $$ = action.fullIdentiferAddPath(action.fullIdentiferCreate(), $1);
                                                                        }
                |   ObjectNamePart '.' ObjectIdentifer                  {
                                                                            action.log("ObjectNamePart:     ObjectNamePart . ObjectIdentifer");
                                                                            $$ = action.fullIdentiferAddPath($3, $1);
                                                                        }

TypeName:           TypeNameFull                                        {
                                                                            action.log("TypeName:           TypeNameFull");
                                                                            $$ = action.findTypeInScope($1);
                                                                        }
TypeNameFull:       TypeIdentifer                                       {
                                                                            action.log("TypeNameFull:       TypeIdentifer");
                                                                            $$ = action.fullIdentiferAddPath(action.fullIdentiferCreate(), $1);
                                                                        }
                |   NameSpaceIdentifer SCOPE TypeNameFull               {
                                                                            action.log("TypeNameFull:       NameSpaceIdentifer :: TypeNameFull");
                                                                            $$ = action.fullIdentiferAddPath($3, $1);
                                                                        }
                |   TypeIdentifer SCOPE TypeNameFull                    {
                                                                            action.log("TypeNameFull:        TypeIdentifer :: TypeNameFull");
                                                                            $$ = action.fullIdentiferAddPath($3, $1);
                                                                        }


Literal:            STRING                                              {
                                                                            action.log("Literal:            STRING");
                                                                            $$ = action.addLiteralString();
                                                                        }

NameSpaceIdentifer: Identifer                                           {
                                                                            action.log("NameSpaceIdentifer: Identifer");
                                                                            $$ = action.identifierCheckNS($1);
                                                                        }
TypeIdentifer:      Identifer                                           {
                                                                            action.log("TypeIdentifer:      Identifer");
                                                                            $$ = action.identifierCheckType($1);
                                                                        }
ObjectIdentifer:    Identifer                                           {
                                                                            action.log("ObjectIdentifer:    Identifer");
                                                                            $$ = action.identifierCheckObject($1);
                                                                        }

Identifer:          IDENTIFIER                                          {
                                                                            action.log("Identifer:          IDENTIFIER");
                                                                            $$ = action.identifierCreate();
                                                                        }



%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithAction(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

