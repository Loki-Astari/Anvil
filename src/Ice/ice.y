
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
int yylex(void*, ThorsAnvil::Anvil::Ice::Lexer& lexer);
//}

#define YYSTYPE ThorsAnvil::Anvil::Ice::Int
//#define YYDEBUG 1

%}


%parse-param {Lexer&  lexer}
%parse-param {Action& action}
%lex-param   {Lexer&  lexer}


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

Anvil:              NamespaceList                                       {action.log("Anvil:              NamespaceList");$$ = 0;YYACCEPT;}


NamespaceList:      Namespace                                           {action.log("NamespaceList:      Namespace");}
                |   NamespaceList Namespace                             {action.log("NamespaceList:      NamespaceList Namespace");}

Namespace:          NAMESPACE NameSpaceIdentifer '{'                    {
                                                                            action.log("Namespace:          NAMESPACE NameSpaceIdentifer { DeclListOpt } P1A");
                                                                            $$ = action.scopeAddNamespace(lexer, $2);
                                                                        }
                                                     DeclListOpt '}'    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P2A");
                                                                            $$ = action.scopeClose(lexer, $4);
                                                                        }

DeclListOpt:                                                            {action.log("DeclListOpt:        >EMPTY<");}
                |   DeclList                                            {action.log("DeclListOpt:        DeclList");}
DeclList:           Decl                                                {action.log("DeclList:           Decl");}
                |   DeclList Decl                                       {action.log("DeclList:           DeclList Decl");}

Decl:               NAMESPACE NameSpaceIdentifer '{'                    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P1B");
                                                                            $$ = action.scopeAddNamespace(lexer, $2);
                                                                        }
                                                     DeclListOpt '}'    {
                                                                            action.log("Decl:               NAMESPACE NameSpaceIdentifer { DeclListOpt } P2B");
                                                                            action.releaseStorage(action.scopeClose(lexer, $4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }

                |   CLASS   TypeIdentifer '{'                           {
                                                                            action.log("Decl:               CLASS TypeIdentifer { DeclListOpt } P1");
                                                                            $$ = action.scopeAddClass(lexer, $2);
                                                                        }
                                             DeclListOpt '}'            {
                                                                            action.log("Decl:               CLASS TypeIdentifer { DeclListOpt } P2");
                                                                            action.releaseStorage(action.scopeClose(lexer, $4));
                                                                            action.assertNoStorage($5);
                                                                            $$ = 0;
                                                                        }
                |   ARRAY  TypeIdentifer '{' ObjectDecl '}'             {
                                                                            action.log("Decl:               ARRAY TypeIdentifer { ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddArray(lexer, $2, $4));
                                                                            $$ = 0;
                                                                        }
                |   MAP    TypeIdentifer '{' ObjectDecl ',' ObjectDecl '}' {
                                                                            action.log("Decl:               MAP TypeIdentifer { ObjectDecl , ObjectDecl }");
                                                                            action.releaseStorage(action.scopeAddMap(lexer, $2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   FUNC   TypeIdentifer '{' ParamListOpt ARROW ObjectDecl '}' {
                                                                            action.log("Decl:               FUNC TypeIdentifer ( ParamListOpt ) -> ObjectDecl ;");
                                                                            action.releaseStorage(action.scopeAddFunc(lexer, $2, $4, $6));
                                                                            $$ = 0;
                                                                        }
                |   ObjectIdentifer ':' ObjectDecl InitObject           {
                                                                            action.log("Decl:               ObjectIdentifer : ObjectDecl InitObject");
                                                                            // TODO
                                                                            // action.releaseStorage(action.scopeAddObject(lexer, $1, $3));
                                                                            $$ = 0;
                                                                        }
                |   Statement                                           {
                                                                            action.log("Decl:               Statement");
                                                                            action.releaseStorage(action.scopeAddStatement(lexer, $1));
                                                                            $$ = 0;
                                                                        }

ParamListOpt:                                                           {
                                                                            action.log("ParamListOpt:       >EMPTY<");
                                                                            $$ = action.paramListCreate(lexer);
                                                                        }
                |   ParamList                                           {
                                                                            action.log("ParamListOpt:       ParamList");
                                                                            $$ = $1;
                                                                        }
ParamList:          ObjectDecl                                          {
                                                                            action.log("ParamList:          ObjectDecl");
                                                                            $$ = action.paramListAdd(lexer, action.paramListCreate(lexer), $1);
                                                                        }
                |   ParamList ',' ObjectDecl                            {
                                                                            action.log("ParamList:          ParamList , ObjectDecl");
                                                                            $$ = action.paramListAdd(lexer, $1, $3);
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
                                                                            $$ = action.scopeAddClass(lexer, action.generateAnonName());
                                                                        }
                               DeclListOpt '}'                          {
                                                                            action.log("AnonDecl:           CLASS { DeclListOpt } P2");
                                                                            $$ = action.scopeClose(lexer, $3);
                                                                            action.assertNoStorage($4);
                                                                        }
                |   ARRAY  '{' ObjectDecl '}'                           {
                                                                            action.log("AnonDecl:           ARRAY { ObjectDecl }");
                                                                            $$ = action.scopeAddArray(lexer, action.generateAnonName(), $3);
                                                                        }
                |   MAP    '{' ObjectDecl ',' ObjectDecl '}'            {
                                                                            action.log("AnonDecl:           MAP { ObjectDecl , ObjectDecl }");
                                                                            $$ = action.scopeAddMap(lexer, action.generateAnonName(), $3, $5);
                                                                        }
                |   FUNC   '{' ParamListOpt ARROW ObjectDecl '}'        {
                                                                            action.log("AnonDecl:           FUNC { ParamListOpt -> ObjectDecl }");
                                                                            $$ = action.scopeAddFunc(lexer, action.generateAnonName(), $3, $5);
                                                                        }

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

TypeName:           TypeNameFull                                        {
                                                                            action.log("TypeName:           TypeNameFull");
                                                                            $$ = action.findTypeInScope(lexer, $1);
                                                                        }
TypeNameFull:       TypeIdentifer                                       {
                                                                            action.log("TypeNameFull:       TypeIdentifer");
                                                                            $$ = action.fullIdentiferAddPath(lexer, action.fullIdentiferCreate(lexer), $1);
                                                                        }
                |   NameSpaceIdentifer SCOPE TypeNameFull               {
                                                                            action.log("TypeNameFull:       NameSpaceIdentifer :: TypeNameFull");
                                                                            $$ = action.fullIdentiferAddPath(lexer, $3, $1);
                                                                        }
                |   TypeIdentifer SCOPE TypeNameFull                    {
                                                                            action.log("TypeNameFull:        TypeIdentifer :: TypeNameFull");
                                                                            $$ = action.fullIdentiferAddPath(lexer, $3, $1);
                                                                        }


Literal:            STRING


NameSpaceIdentifer: Identifer                                           {
                                                                            action.log("NameSpaceIdentifer: Identifer");
                                                                            $$ = action.identifierCheckNS(lexer, $1);
                                                                        }
TypeIdentifer:      Identifer                                           {
                                                                            action.log("TypeIdentifer:      Identifer");
                                                                            $$ = action.identifierCheckType(lexer, $1);
                                                                        }
ObjectIdentifer:    Identifer                                           {
                                                                            action.log("ObjectIdentifer:    Identifer");
                                                                            $$ = action.identifierCheckObject(lexer, $1);
                                                                        }

Identifer:          IDENTIFIER                                          {
                                                                            action.log("Identifer:          IDENTIFIER");
                                                                            $$ = action.identifierCreate(lexer);
                                                                        }



%%


int yylex(void*, Lexer& lexer)
{
    return lexer.yylex();
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(lexer, msg);
}

