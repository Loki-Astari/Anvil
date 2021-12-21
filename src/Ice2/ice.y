
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
using ThorsAnvil::Anvil::Ice::ParamValueListId;
using ThorsAnvil::Anvil::Ice::StatementListId;
using ThorsAnvil::Anvil::Ice::DeclId;
using ThorsAnvil::Anvil::Ice::ScopeId;
using ThorsAnvil::Anvil::Ice::NamespaceId;
using ThorsAnvil::Anvil::Ice::TypeId;
using ThorsAnvil::Anvil::Ice::ClassId;
using ThorsAnvil::Anvil::Ice::FunctionId;
using ThorsAnvil::Anvil::Ice::ObjectId;
using ThorsAnvil::Anvil::Ice::IdentifierId;
using ThorsAnvil::Anvil::Ice::ObjectInitId;
using ThorsAnvil::Anvil::Ice::StatementId;
using ThorsAnvil::Anvil::Ice::ExpressionId;
// ---
using ThorsAnvil::Anvil::Ice::Decl;
using ThorsAnvil::Anvil::Ice::Scope;
using ThorsAnvil::Anvil::Ice::Namespace;
using ThorsAnvil::Anvil::Ice::Type;
using ThorsAnvil::Anvil::Ice::Class;
using ThorsAnvil::Anvil::Ice::Function;
using ThorsAnvil::Anvil::Ice::Object;
using ThorsAnvil::Anvil::Ice::ObjectInit;
%}
%union {
    semantic_type(): voidId{0} {};
    VoidId              voidId;
    DeclListId          declListId;
    NamespaceListId     namespaceListId;
    ParamListId         paramListId;
    ParamValueListId    paramValueListId;
    StatementListId     statementListId;
    DeclId              declId;
    NamespaceId         namespaceId;
    ScopeId             scopeId;
    TypeId              typeId;
    ClassId             classId;
    FunctionId          functionId;
    ObjectId            objectId;
    IdentifierId        identifierId;
    ObjectInitId        objectInitId;
    StatementId         statementId;
    ExpressionId        expressionId;
}

%token                          NAMESPACE
%token                          CLASS
%token                          FUNC
%token                          ARROW
%token                          SCOPE
%token                          IDENTIFIER_NS
%token                          IDENTIFIER_TYPE
%token                          IDENTIFIER_OBJECT
%token                          LITERAL_STRING
%token                          OP_ASSIGN_MUL
%token                          OP_ASSIGN_DIV
%token                          OP_ASSIGN_MOD
%token                          OP_ASSIGN_ADD
%token                          OP_ASSIGN_SUB
%token                          OP_ASSIGN_L
%token                          OP_ASSIGN_R
%token                          OP_ASSIGN_AND
%token                          OP_ASSIGN_XOR
%token                          OP_ASSIGN_OR
%token                          OP_AND
%token                          OP_OR
%token                          OP_L_SHIFT
%token                          OP_R_SHIFT
%token                          OP_EQ
%token                          OP_NE
%token                          OP_LE
%token                          OP_GE
%token                          OP_INC
%token                          OP_DEC
%type   <voidId>                Anvil
%type   <declListId>            DeclListOpt
%type   <declListId>            DeclList
%type   <namespaceListId>       NamespaceList
%type   <paramListId>           ParamListOpt
%type   <paramListId>           ParamList
%type   <paramValueListId>      ParamValueListOpt
%type   <paramValueListId>      ParamValueList
%type   <statementListId>       StatementListOpt
%type   <statementListId>       StatementList
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
%type   <objectId>              ObjectDecl
%type   <identifierId>          ScopeIdentifier
%type   <identifierId>          IdentifierNamespace
%type   <identifierId>          IdentifierType
%type   <identifierId>          IdentifierObject
%type   <objectInitId>          ObjectInit;
%type   <statementId>           Statement
%type   <expressionId>          Expression
%type   <expressionId>          AssignmentExpression
%type   <expressionId>          ConditionalExpression
%type   <expressionId>          LogicalOrExpression
%type   <expressionId>          LogicalAndExpression
%type   <expressionId>          InclusiveOrExpression
%type   <expressionId>          ExclusiveOrExpression
%type   <expressionId>          AndExpression
%type   <expressionId>          EqualityExpression
%type   <expressionId>          RelationalExpression
%type   <expressionId>          ShiftExpression
%type   <expressionId>          AddExpression
%type   <expressionId>          MultExpression
%type   <expressionId>          CastExpression
%type   <expressionId>          UnaryExpression
%type   <expressionId>          PostFixExpression
%type   <expressionId>          PrimaryExpression
%type   <expressionId>          Literal


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

ParamValueListOpt:                                                          {$$ = action.listParamValueCreate();}
                    |   ParamValueList                                      {$$ = $1;}
ParamValueList:         Expression                                          {$$ = action.listParamValueAppend(action.listParamValueCreate(), $1);}
                    |   ParamValueList ',' Expression                       {$$ = action.listParamValueAppend($1, $3);}

StatementListOpt:                                                           {$$ = action.listStatementCreate();}
                    |   StatementList                                       {$$ = $1;}
StatementList:          Statement                                           {$$ = action.listStatementAppend(action.listStatementCreate(), $1);}
                    |   StatementList Statement                             {$$ = action.listStatementAppend($1, $2);}

Namespace:              NamespaceStart '{' DeclListOpt  '}'                 {$$ = action.scopeNamespaceClose($1, $3);}
NamespaceStart:         NAMESPACE IdentifierNamespace                       {$$ = action.scopeNamespaceOpen($2);}

Decl:                   Namespace                                           {$$ = action.convert<Namespace, Decl>($1);}
                    |   Class                                               {$$ = action.convert<Class, Decl>($1);}
                    |   Function                                            {$$ = action.convert<Function, Decl>($1);}
                    |   ObjectDecl                                          {$$ = action.convert<Object, Decl>($1);}

Class:                  ClassStart '{' DeclListOpt '}'                      {$$ = action.scopeClassClose($1, $3);}
ClassStart:             CLASS IdentifierType                                {$$ = action.scopeClassOpen($2);}
ClassAnon:              CLASS '{' DeclListOpt '}'                           {$$ = action.scopeClassAnon($3);}

Function:               FunctionStart '{' ParamListOpt ARROW Type '}'       {$$ = action.scopeFunctionClose($1, $3, $5);}
FunctionStart:          FUNC IdentifierType                                 {$$ = action.scopeFunctionOpen($2);}
FunctionAnon:           FUNC '{' ParamListOpt ARROW Type '}'                {$$ = action.scopeFunctionAnon($3, $5);}

ObjectDecl:             IdentifierObject ':' TypeDecl ObjectInit            {$$ = action.scopeObjectAdd($1, $3, $4);}
ObjectInit:             ';'                                                 {$$ = action.initVariable(action.listParamValueCreate());}
                    |   '(' ParamValueListOpt ')' ';'                       {$$ = action.initVariable($2);}
                    |   '{' StatementListOpt '}'                            {$$ = action.initFunction($2);}

Statement:              Expression ';'                                      {$$ = action.statmentExpression($1);}

TypeDecl:               Type                                                {$$ = $1;}
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

Expression:             AssignmentExpression                                {$$ = $1;}

AssignmentExpression:   ConditionalExpression                               {$$ = $1;}
                    |   UnaryExpression '='           AssignmentExpression  {$$ = action.expressionAssign($1, $3);}
                    |   UnaryExpression OP_ASSIGN_MUL AssignmentExpression  {$$ = action.expressionAssignMul($1, $3);}
                    |   UnaryExpression OP_ASSIGN_DIV AssignmentExpression  {$$ = action.expressionAssignDiv($1, $3);}
                    |   UnaryExpression OP_ASSIGN_MOD AssignmentExpression  {$$ = action.expressionAssignMod($1, $3);}
                    |   UnaryExpression OP_ASSIGN_ADD AssignmentExpression  {$$ = action.expressionAssignAdd($1, $3);}
                    |   UnaryExpression OP_ASSIGN_SUB AssignmentExpression  {$$ = action.expressionAssignSub($1, $3);}
                    |   UnaryExpression OP_ASSIGN_L   AssignmentExpression  {$$ = action.expressionAssignLSh($1, $3);}
                    |   UnaryExpression OP_ASSIGN_R   AssignmentExpression  {$$ = action.expressionAssignRSh($1, $3);}
                    |   UnaryExpression OP_ASSIGN_AND AssignmentExpression  {$$ = action.expressionAssignAnd($1, $3);}
                    |   UnaryExpression OP_ASSIGN_XOR AssignmentExpression  {$$ = action.expressionAssignXOR($1, $3);}
                    |   UnaryExpression OP_ASSIGN_OR  AssignmentExpression  {$$ = action.expressionAssignOR($1, $3);}

ConditionalExpression:  LogicalOrExpression                                 {$$ = $1;}
                    |   LogicalOrExpression
                        '?' Expression ':' ConditionalExpression            {$$ = action.expressionConditional($1, $3, $5);}

LogicalOrExpression:    LogicalAndExpression                                {$$ = $1;}
                    |   LogicalOrExpression OP_OR LogicalAndExpression      {$$ = action.expressionLogicalOr($1, $3);}

LogicalAndExpression:   InclusiveOrExpression                               {$$ = $1;}
                    |   LogicalAndExpression OP_AND InclusiveOrExpression   {$$ = action.expressionLogicalAnd($1, $3);}

InclusiveOrExpression:  ExclusiveOrExpression                               {$$ = $1;}
                    |   InclusiveOrExpression '|' ExclusiveOrExpression     {$$ = action.expressionInclusiveOr($1, $3);}

ExclusiveOrExpression:  AndExpression                                       {$$ = $1;}
                    |   ExclusiveOrExpression '^' AndExpression             {$$ = action.expressionExclusiveOr($1, $3);}

AndExpression:          EqualityExpression                                  {$$ = $1;}
                    |   AndExpression '&' EqualityExpression                {$$ = action.expressionAnd($1, $3);}

EqualityExpression:     RelationalExpression                                {$$ = $1;}
                    |   EqualityExpression OP_EQ RelationalExpression       {$$ = action.expressionEqual($1, $3);}
                    |   EqualityExpression OP_NE RelationalExpression       {$$ = action.expressionNotEqual($1, $3);}

RelationalExpression:   ShiftExpression                                     {$$ = $1;}
                    |   RelationalExpression '<' ShiftExpression            {$$ = action.expressionLess($1, $3);}
                    |   RelationalExpression '>' ShiftExpression            {$$ = action.expressionGreat($1, $3);}
                    |   RelationalExpression OP_LE ShiftExpression          {$$ = action.expressionLessEqual($1, $3);}
                    |   RelationalExpression OP_GE ShiftExpression          {$$ = action.expressionGreatEqual($1, $3);}

ShiftExpression:        AddExpression                                       {$$ = $1;}
                    |   ShiftExpression OP_L_SHIFT AddExpression            {$$ = action.expressionShiftLeft($1, $3);}
                    |   ShiftExpression OP_R_SHIFT AddExpression            {$$ = action.expressionShiftRight($1, $3);}

AddExpression:          MultExpression                                      {$$ = $1;}
                    |   AddExpression '+' MultExpression                    {$$ = action.expressionAdd($1, $3);}
                    |   AddExpression '-' MultExpression                    {$$ = action.expressionSub($1, $3);}

MultExpression:         CastExpression                                      {$$ = $1;}
                    |   MultExpression '*' CastExpression                   {$$ = action.expressionMul($1, $3);}
                    |   MultExpression '/' CastExpression                   {$$ = action.expressionDiv($1, $3);}
                    |   MultExpression '%' CastExpression                   {$$ = action.expressionMod($1, $3);}

CastExpression:         UnaryExpression                                     {$$ = $1;}

UnaryExpression:        PostFixExpression                                   {$$ = $1;}
                    |   OP_INC UnaryExpression                              {$$ = action.expressionPreInc($2);}
                    |   OP_DEC UnaryExpression                              {$$ = action.expressionPreDec($2);}
                    |   '+' UnaryExpression                                 {$$ = action.expressionPlus($2);}
                    |   '-' UnaryExpression                                 {$$ = action.expressionNeg($2);}
                    |   '~' UnaryExpression                                 {$$ = action.expressionOneCompliment($2);}
                    |   '!' UnaryExpression                                 {$$ = action.expressionNot($2);}

PostFixExpression:      PrimaryExpression                                   {$$ = $1;}
                    |   PostFixExpression '[' Expression ']'                {$$ = action.expressionArrayAccess($1, $3);}
                    |   PostFixExpression '(' ParamValueListOpt ')'         {$$ = action.expressionFuncCall($1, $3);}
                    |   PostFixExpression '.' IdentifierObject              {$$ = action.expressionMemberAccess($1, $3);}
                    |   PostFixExpression OP_INC                            {$$ = action.expressionPostInc($1);}
                    |   PostFixExpression OP_DEC                            {$$ = action.expressionPostDec($1);}

PrimaryExpression:      IdentifierObject                                    {$$ = action.expreesionFindObjectByName($1);}
                    |   Literal                                             {$$ = $1;}
                    |   '(' Expression ')'                                  {$$ = $2;}

Literal:                LITERAL_STRING                                      {$$ = action.expressionLiteralString();}



%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithActionGo(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

