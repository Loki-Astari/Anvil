
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
using ThorsAnvil::Anvil::Ice::DeclId;
using ThorsAnvil::Anvil::Ice::NamespaceId;
using ThorsAnvil::Anvil::Ice::ClassId;
using ThorsAnvil::Anvil::Ice::IdentifierId;
// ---
using ThorsAnvil::Anvil::Ice::Decl;
using ThorsAnvil::Anvil::Ice::Namespace;
using ThorsAnvil::Anvil::Ice::Class;
%}
%union {
    semantic_type(): voidId{0} {};
    VoidId              voidId;
    DeclListId          declListId;
    NamespaceListId     namespaceListId;
    DeclId              declId;
    NamespaceId         namespaceId;
    ClassId             classId;
    IdentifierId        identifierId;
}

%token                          NAMESPACE
%token                          CLASS
%token                          IDENTIFIER_NS
%token                          IDENTIFIER_TYPE
%type   <voidId>                Anvil
%type   <namespaceListId>       NamespaceList
%type   <namespaceId>           Namespace
%type   <namespaceId>           NamespaceStart
%type   <classId>               Class
%type   <classId>               ClassStart
%type   <declListId>            DeclListOpt
%type   <declListId>            DeclList
%type   <declId>                Decl
%type   <declId>                NamespaceDecl
%type   <declId>                ClassDecl
%type   <identifierId>          IdentifierNamespace
%type   <identifierId>          IdentifierType



%%

Anvil:                                                          {$$ = action.anvilProgram(action.listNamespaceCreate());}
                    |   NamespaceList                           {$$ = action.anvilProgram($1);}

NamespaceList:          Namespace                               {$$ = action.listNamespaceAppend(action.listNamespaceCreate(), $1);}
                    |   NamespaceList Namespace                 {$$ = action.listNamespaceAppend($1, $2);}

Namespace:              NamespaceStart '{' DeclListOpt  '}'     {$$ = action.scopeNamespaceClose($1, $3);}
NamespaceStart:         NAMESPACE IdentifierNamespace           {$$ = action.scopeNamespaceOpen($2);}

DeclListOpt:                                                    {$$ = action.listDeclCreate();}
                    |   DeclList                                {$$ = $1;}

DeclList:               Decl                                    {$$ = action.listDeclAppend(action.listDeclCreate(), $1);}
                    |   DeclList Decl                           {$$ = action.listDeclAppend($1, $2);}

Decl:                   NamespaceDecl                           {$$ = $1;}
                    |   ClassDecl                               {$$ = $1;}

NamespaceDecl:          Namespace                               {$$ = action.convert<Namespace, Decl>($1);}
ClassDecl:              Class                                   {$$ = action.convert<Class, Decl>($1);}

Class:                  ClassStart '{' DeclListOpt '}'          {$$ = action.scopeClassClose($1, $3);}
ClassStart:             CLASS IdentifierType                    {$$ = action.scopeClassOpen($2);}


IdentifierNamespace:    IDENTIFIER_NS                           {$$ = action.identifierCreate();}
IdentifierType:         IDENTIFIER_TYPE                         {$$ = action.identifierCreate();}

%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithActionGo(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

