
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
using ThorsAnvil::Anvil::Ice::TypeListId;
using ThorsAnvil::Anvil::Ice::ExpressionListId;
using ThorsAnvil::Anvil::Ice::StatementListId;
using ThorsAnvil::Anvil::Ice::DeclId;
using ThorsAnvil::Anvil::Ice::ScopeId;
using ThorsAnvil::Anvil::Ice::NamespaceId;
using ThorsAnvil::Anvil::Ice::TypeId;
using ThorsAnvil::Anvil::Ice::ClassId;
using ThorsAnvil::Anvil::Ice::FunctionId;
using ThorsAnvil::Anvil::Ice::CodeBlockId;
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
using ThorsAnvil::Anvil::Ice::Statement;
using ThorsAnvil::Anvil::Ice::Expression;
using ThorsAnvil::Anvil::Ice::Id;
%}
%union {
    semantic_type(): voidId{0} {};
    VoidId              voidId;
    DeclListId          declListId;
    NamespaceListId     namespaceListId;
    TypeListId          typeListId;
    ExpressionListId    expressionListId;
    StatementListId     statementListId;
    DeclId              declId;
    NamespaceId         namespaceId;
    ScopeId             scopeId;
    TypeId              typeId;
    ClassId             classId;
    FunctionId          functionId;
    CodeBlockId         codeBlockId;
    ObjectId            objectId;
    IdentifierId        identifierId;
    ObjectInitId        objectInitId;
    StatementId         statementId;
    ExpressionId        expressionId;
    Id<std::string>     assembley;
}

%token                          ASSEMBLEY_START
%token                          ASSEMBLEY_END
%token                          ASSEMBLEY_LINE
%token                          NAMESPACE
%token                          CLASS
%token                          FUNC
%token                          RETURN
%token                          CONSTRUCT
%token                          DESTRUCT
%token                          ARROW
%token                          SCOPE
%token                          IDENTIFIER_NS
%token                          IDENTIFIER_TYPE
%token                          IDENTIFIER_OBJECT
%token                          LITERAL_STRING
%token                          LITERAL_INTEGER
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
%type   <typeListId>            TypeListOpt
%type   <typeListId>            TypeList
%type   <expressionListId>      ExpressionListOpt
%type   <expressionListId>      ExpressionList
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
%type   <objectId>              Object
%type   <objectId>              Constructor
%type   <functionId>            ConstructorStart
%type   <objectId>              Destructor
%type   <functionId>            DestructorStart
%type   <identifierId>          ScopeIdentifier
%type   <identifierId>          IdentifierNamespace
%type   <identifierId>          IdentifierType
%type   <identifierId>          IdentifierObject
%type   <objectInitId>          ObjectInit
%type   <statementId>           Statement
%type   <statementId>           CodeBlock
%type   <codeBlockId>           CodeBlockStart
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
%type   <assembley>             Assembley
%type   <assembley>             AssembleyLines



%%

Anvil:                                                                      {$$ = action.anvilProgram(action.listCreate<Namespace>());}
                    |   NamespaceList                                       {$$ = action.anvilProgram($1);}

NamespaceList:          Namespace                                           {$$ = action.listAppend<Namespace>(action.listCreate<Namespace>(), $1);}
                    |   NamespaceList Namespace                             {$$ = action.listAppend<Namespace>($1, $2);}

DeclListOpt:                                                                {$$ = action.listCreate<Decl>();}
                    |   DeclList                                            {$$ = $1;}
DeclList:               Decl                                                {$$ = action.listAppend<Decl>(action.listCreate<Decl>(), $1);}
                    |   DeclList Decl                                       {$$ = action.listAppend<Decl>($1, $2);}

TypeListOpt:                                                                {$$ = action.listCreate<Type>();}
                    |   TypeList                                            {$$ = $1;}
TypeList:               Type                                                {$$ = action.listAppend<Type>(action.listCreate<Type>(), $1);}
                    |   TypeList ',' Type                                   {$$ = action.listAppend<Type>($1, $3);}

ExpressionListOpt:                                                          {$$ = action.listCreate<Expression>();}
                    |   ExpressionList                                      {$$ = $1;}
ExpressionList:         Expression                                          {$$ = action.listAppend<Expression>(action.listCreate<Expression>(), $1);}
                    |   ExpressionList ',' Expression                       {$$ = action.listAppend<Expression>($1, $3);}

StatementListOpt:                                                           {$$ = action.listCreate<Statement>();}
                    |   StatementList                                       {$$ = $1;}
StatementList:          Statement                                           {$$ = action.listAppend<Statement>(action.listCreate<Statement>(), $1);}
                    |   StatementList Statement                             {$$ = action.listAppend<Statement>($1, $2);}

Namespace:              NamespaceStart '{' DeclListOpt  '}'                 {$$ = action.scopeNamespaceClose($1, $3);}
NamespaceStart:         NAMESPACE IdentifierNamespace                       {$$ = action.scopeNamespaceOpen($2);}

Decl:                   Namespace                                           {$$ = action.convert<Namespace, Decl>($1);}
                    |   Class                                               {$$ = action.convert<Class, Decl>($1);}
                    |   Function                                            {$$ = action.convert<Function, Decl>($1);}
                    |   Constructor                                         {$$ = action.convert<Object, Decl>($1);}
                    |   Destructor                                          {$$ = action.convert<Object, Decl>($1);}
                    |   ObjectDecl                                          {$$ = action.convert<Object, Decl>($1);}

Class:                  ClassStart '{' DeclListOpt '}'                      {$$ = action.scopeClassClose($1, $3);}
ClassStart:             CLASS IdentifierType                                {$$ = action.scopeClassOpen($2);}
ClassAnon:              CLASS '{' DeclListOpt '}'                           {$$ = action.scopeClassAnon($3);}

Function:               FunctionStart '{' TypeListOpt ARROW Type '}'        {$$ = action.scopeFunctionClose($1, $3, $5);}
FunctionStart:          FUNC IdentifierType                                 {$$ = action.scopeFunctionOpen($2);}
FunctionAnon:           FUNC '{' TypeListOpt ARROW Type '}'                 {$$ = action.scopeFunctionAnon($3, $5);}

Constructor:            ConstructorStart '{' TypeListOpt '}' CodeBlock      {$$ = action.scopeConstructorAdd($1, $3, $5);}
ConstructorStart:       CONSTRUCT                                           {$$ = action.scopeConstructorInit();}
Destructor:             DestructorStart '{' TypeListOpt '}' CodeBlock       {$$ = action.scopeDestructorAdd($1, $3, $5);}
DestructorStart:        DESTRUCT                                            {$$ = action.scopeDestructorInit();}

ObjectDecl:             IdentifierObject ':' TypeDecl ObjectInit            {$$ = action.scopeObjectAdd($1, $3, $4);}
ObjectInit:             ';'                                                 {$$ = action.initVariable(action.listCreate<Expression>());}
                    |   '(' ExpressionListOpt ')' ';'                       {$$ = action.initVariable($2);}
                    |   CodeBlock                                           {$$ = action.initFunction($1);}


Statement:              Expression ';'                                      {$$ = action.statmentExpression($1);}
                    |   RETURN Expression ';'                               {$$ = action.statmentReturn($2);}
                    |   Assembley                                           {$$ = action.statmentAssembley($1);}
                    |   CodeBlock

CodeBlock:              CodeBlockStart StatementListOpt '}'                 {$$ = action.scopeCodeBlockClose($1, $2);}
CodeBlockStart:         '{'                                                 {$$ = action.scopeCodeBlockOpen();}

TypeDecl:               Type                                                {$$ = $1;}
                    |   AnonType                                            {$$ = $1;}

AnonType:               ClassAnon                                           {$$ = action.convert<Class, Type>($1);}
                    |   FunctionAnon                                        {$$ = action.convert<Function, Type>($1);}

Type:                   IdentifierType                                      {$$ = action.getNameFromScopeStack<Type>($1);}
                    |   ScopedType SCOPE IdentifierType                     {$$ = action.getNameFromScope<Type>($1, $3);}

Object:                 IdentifierObject                                    {$$ = action.getNameFromScopeStack<Object>($1);}
                    |   ScopedType SCOPE IdentifierObject                   {$$ = action.getNameFromScope<Object>($1, $3);}

ScopedType:             ScopeIdentifier                                     {$$ = action.getNameFromScopeStack<Scope>($1);}
                    |   ScopedType SCOPE ScopeIdentifier                    {$$ = action.getNameFromScope<Scope>($1, $3);}

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
                    |   PostFixExpression '(' ExpressionListOpt ')'         {$$ = action.expressionFuncCall($1, $3);}
                    |   PostFixExpression '.' IdentifierObject              {$$ = action.expressionMemberAccess($1, $3);}
                    |   PostFixExpression OP_INC                            {$$ = action.expressionPostInc($1);}
                    |   PostFixExpression OP_DEC                            {$$ = action.expressionPostDec($1);}

PrimaryExpression:      Object                                              {$$ = action.expressionObject($1);}
                    |   Literal                                             {$$ = $1;}
                    |   '(' Expression ')'                                  {$$ = $2;}

Literal:                LITERAL_STRING                                      {$$ = action.expressionLiteralString();}
                    |   LITERAL_INTEGER                                     {$$ = action.expressionLiteralInt();}

Assembley:              ASSEMBLEY_START AssembleyLines ASSEMBLEY_END        {$$ = $2;}
AssembleyLines:         ASSEMBLEY_LINE                                      {$$ = action.assemblyInit();}
                    |   AssembleyLines ASSEMBLEY_LINE                       {$$ = $1; action.assembleyAppend($$);}

%%


int yylex(void*, Lexer& lexer, Action& action)
{
    return lexer.yylexWithActionGo(action);
}

void yy::Parser::error(yy::location const& /*location*/, std::string const& msg)
{
    action.error(msg);
}

