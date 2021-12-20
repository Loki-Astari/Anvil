
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
using ThorsAnvil::Anvil::Ice::NamespaceListId;
using ThorsAnvil::Anvil::Ice::NamespaceId;
using ThorsAnvil::Anvil::Ice::IdentifierId;
%}
%union {
    semantic_type(): voidId{0} {};
    VoidId              voidId;
    NamespaceListId     namespaceListId;
    NamespaceId         namespaceId;
    IdentifierId        identifierId;
}

%token                          NAMESPACE
%token                          IDENTIFIER_NS
%type   <voidId>                Anvil
%type   <namespaceListId>       NamespaceList
%type   <namespaceId>           Namespace
%type   <namespaceId>           NamespaceStart
%type   <identifierId>          IdentifierNamespace



%%

Anvil:                                                          {$$ = action.anvilProgram(action.listNamespaceCreate());}
                    |   NamespaceList                           {$$ = action.anvilProgram($1);}

NamespaceList:          Namespace                               {$$ = action.listNamespaceAppend(action.listNamespaceCreate(), $1);}
                    |   NamespaceList Namespace                 {$$ = action.listNamespaceAppend($1, $2);}

Namespace:              NamespaceStart '{' '}'                  {$$ = action.scopeNamespaceClose($1);}
NamespaceStart:         NAMESPACE IdentifierNamespace           {$$ = action.scopeNamespaceOpen($2);}

IdentifierNamespace:    IDENTIFIER_NS                           {$$ = action.identifierCreate();}

%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithActionGo(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

