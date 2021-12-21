
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

//#define YYSTYPE ThorsAnvil::Anvil::Ice::Int
//#define YYDEBUG 1

%}


%parse-param {Lexer&  lexer}
%parse-param {Action& action}
%lex-param   {Lexer&  lexer}
%lex-param   {Action& action}

%{
using ThorsAnvil::Anvil::Ice::VoidId;
using ThorsAnvil::Anvil::Ice::DeclListId;
using ThorsAnvil::Anvil::Ice::NamespaceListId;
using ThorsAnvil::Anvil::Ice::ParamListId;
using ThorsAnvil::Anvil::Ice::DeclId;
using ThorsAnvil::Anvil::Ice::ScopeId;
using ThorsAnvil::Anvil::Ice::NamespaceId;
using ThorsAnvil::Anvil::Ice::TypeId;
using ThorsAnvil::Anvil::Ice::ClassId;
using ThorsAnvil::Anvil::Ice::FunctionId;
using ThorsAnvil::Anvil::Ice::ObjectId;
using ThorsAnvil::Anvil::Ice::IdentifierId;
// ---
using ThorsAnvil::Anvil::Ice::Decl;
using ThorsAnvil::Anvil::Ice::Scope;
using ThorsAnvil::Anvil::Ice::Namespace;
using ThorsAnvil::Anvil::Ice::Type;
using ThorsAnvil::Anvil::Ice::Class;
using ThorsAnvil::Anvil::Ice::Function;
using ThorsAnvil::Anvil::Ice::Object;
%}
%union {
    semantic_type(): voidId{0} {};
    VoidId              voidId;
    DeclListId          declListId;
    NamespaceListId     namespaceListId;
    ParamListId         paramListId;
    DeclId              declId;
    NamespaceId         namespaceId;
    ScopeId             scopeId;
    TypeId              typeId;
    ClassId             classId;
    FunctionId          functionId;
    ObjectId            objectId;
    IdentifierId        identifierId;
}

%token                          NAMESPACE
%token                          CLASS
%token                          FUNC
%token                          ARROW
%token                          SCOPE
%token                          IDENTIFIER_NS
%token                          IDENTIFIER_TYPE
%token                          IDENTIFIER_OBJECT
%type   <voidId>                Anvil
%type   <declListId>            DeclListOpt
%type   <declListId>            DeclList
%type   <namespaceListId>       NamespaceList
%type   <paramListId>           ParamListOpt
%type   <paramListId>           ParamList
%type   <declId>                Decl
%type   <namespaceId>           Namespace
%type   <namespaceId>           NamespaceStart
%type   <classId>               Class
%type   <classId>               ClassStart
%type   <classId>               ClassAnon
%type   <functionId>            Function
%type   <functionId>            FunctionStart
%type   <functionId>            FunctionAnon
%type   <scopeId>               ScopedType
%type   <typeId>                TypeDecl
%type   <typeId>                AnonType
%type   <typeId>                Type
%type   <objectId>              Object
%type   <identifierId>          ScopeIdentifier
%type   <identifierId>          IdentifierNamespace
%type   <identifierId>          IdentifierType
%type   <identifierId>          IdentifierObject



%%

Anvil:                                                                      {$$ = action.anvilProgram(action.listNamespaceCreate());}
                    |   NamespaceList                                       {$$ = action.anvilProgram($1);}

NamespaceList:          Namespace                                           {$$ = action.listNamespaceAppend(action.listNamespaceCreate(), $1);}
                    |   NamespaceList Namespace                             {$$ = action.listNamespaceAppend($1, $2);}

DeclListOpt:                                                                {$$ = action.listDeclCreate();}
                    |   DeclList                                            {$$ = $1;}
DeclList:               Decl                                                {$$ = action.listDeclAppend(action.listDeclCreate(), $1);}
                    |   DeclList Decl                                       {$$ = action.listDeclAppend($1, $2);}

ParamListOpt:                                                               {$$ = action.listParamCreate();}
                    |   ParamList                                           {$$ = $1;}
ParamList:              Type                                                {$$ = action.listParamAppend(action.listParamCreate(), $1);}
                    |   ParamList ',' Type                                  {$$ = action.listParamAppend($1, $3);}

Namespace:              NamespaceStart '{' DeclListOpt  '}'                 {$$ = action.scopeNamespaceClose($1, $3);}
NamespaceStart:         NAMESPACE IdentifierNamespace                       {$$ = action.scopeNamespaceOpen($2);}

Decl:                   Namespace                                           {$$ = action.convert<Namespace, Decl>($1);}
                    |   Class                                               {$$ = action.convert<Class, Decl>($1);}
                    |   Function                                            {$$ = action.convert<Function, Decl>($1);}
                    |   Object                                              {$$ = action.convert<Object, Decl>($1);}

Class:                  ClassStart '{' DeclListOpt '}'                      {$$ = action.scopeClassClose($1, $3);}
ClassStart:             CLASS IdentifierType                                {$$ = action.scopeClassOpen($2);}
ClassAnon:              CLASS '{' DeclListOpt '}'                           {$$ = action.scopeClassAnon($3);}

Function:               FunctionStart '{' ParamListOpt ARROW Type '}'       {$$ = action.scopeFunctionClose($1, $3, $5);}
FunctionStart:          FUNC IdentifierType                                 {$$ = action.scopeFunctionOpen($2);}
FunctionAnon:           FUNC '{' ParamListOpt ARROW Type '}'                {$$ = action.scopeFunctionAnon($3, $5);}

Object:                 IdentifierObject ':' TypeDecl                       {$$ = action.scopeObjectAdd($1, $3);}

TypeDecl:               Type ';'                                            {$$ = $1;}
                    |   AnonType                                            {$$ = $1;}

AnonType:               ClassAnon                                           {$$ = action.convert<Class, Type>($1);}
                    |   FunctionAnon                                        {$$ = action.convert<Function, Type>($1);}

Type:                   IdentifierType                                      {$$ = action.getTypeFromName($1);}
                    |   ScopedType SCOPE IdentifierType                     {$$ = action.getTypeFromScope($1, $3);}

ScopedType:             ScopeIdentifier                                     {$$ = action.getScopeFromName($1);}
                    |   ScopedType SCOPE ScopeIdentifier                    {$$ = action.getScopeFromScope($1, $3);}

ScopeIdentifier:        IdentifierNamespace                                 {$$ = $1;}
                    |   IdentifierType                                      {$$ = $1;}


IdentifierNamespace:    IDENTIFIER_NS                                       {$$ = action.identifierCreate();}
IdentifierType:         IDENTIFIER_TYPE                                     {$$ = action.identifierCreate();}
IdentifierObject:       IDENTIFIER_OBJECT                                   {$$ = action.identifierCreate();}

%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithActionGo(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

