#include "Action.h"
#include "Lexer.h"

using namespace ThorsAnvil::Anvil::Ice;

struct ScopePrinter
{
    char const* f;
    ScopePrinter(char const* f)
        : f(f)
    {
        //std::cout << "Entered Scope: " << f << "\n";
    }
    ~ScopePrinter()
    {
        //std::cout << "Left Scope: " << f << "\n";
    }
};

Action::Action(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output)
    : lexer(lexer)
    , storage(storage)
    , globalScope(globalScope)
    , output(output)
    , lineNo(0)
    , offset(0)
{
    currentScope.emplace_back(globalScope);
}

Action::~Action()
{}


// Lexer Stuff
// ========================
void Action::token()
{
    log("Token: ", lexer.lexem(), "\n");
    addToLine();
}

void Action::addToLine()
{
    std::string_view    token = lexer.lexem();

    currentLine += token;
    offset += std::size(token);
}

void Action::resetLine()
{
    currentLine.clear();
    ++lineNo;
    ++offset;
}

// Public Utility
// ========================
IdentifierId Action::anonName()
{
    return storage.add<std::string>(std::string(""));
}

// Parser
// ========================

VoidId Action::anvilProgram(NamespaceListId id)
{
    ScopePrinter scope("anvilProgram");
    NamespaceListAccess     access(storage, id);
    return anvilProgramV(currentScope.back(), moveAccess(access));
}
VoidId Action::anvilProgramV(Scope& /*top*/, NamespaceList /*list*/)
{
    return 0;
}
// ------------------

NamespaceId Action::scopeNamespaceOpen(IdentifierId id)
{
    ScopePrinter scope("scopeNamespaceOpen");
    IdentifierAccess    access(storage, id);
    Namespace& newNameSpace = scopeNamespaceOpenV(currentScope.back(), moveAccess(access));
    return storage.add(NamespaceRef{newNameSpace});
}
Namespace& Action::scopeNamespaceOpenV(Scope& top, Identifier namespaceName)
{
    Namespace& newNameSpace = getOrAddDeclToScope<Namespace>(top, std::move(namespaceName));
    currentScope.emplace_back(newNameSpace);
    return newNameSpace;
}
// ------------------

NamespaceId Action::scopeNamespaceClose(NamespaceId id, DeclListId listId)
{
    ScopePrinter scope("scopeNamespaceClose");
    NamespaceAccess     access(storage, id);
    Namespace&  ns = scopeNamespaceCloseV(currentScope.back(), access, moveAccess(DeclListAccess(storage, listId)));
    return storage.add(NamespaceRef{ns});
}
Namespace& Action::scopeNamespaceCloseV(Scope& top, Namespace& ns, DeclList /*decl*/)
{
    Namespace*      topNS = dynamic_cast<Namespace*>(&top);

    if (&ns != topNS)
    {
        throw std::domain_error("Internal Error: Scope Note correctly aligned from Namespace");
    }
    currentScope.pop_back();
    return ns;
}
// ------------------

ClassId Action::scopeClassOpen(IdentifierId id)
{
    ScopePrinter scope("scopeClassOpen");
    IdentifierAccess     access(storage, id);
    Class& newClass = scopeClassOpenV(currentScope.back(), moveAccess(access));
    return storage.add(ClassRef{newClass});
}
Class& Action::scopeClassOpenV(Scope& top, Identifier className)
{
    Class& newClass = getOrAddDeclToScope<Class>(top, std::move(className));
    currentScope.emplace_back(newClass);

    return newClass;
}
// ------------------

ClassId Action::scopeClassClose(ClassId id, DeclListId listId)
{
    ScopePrinter scope("scopeClassClose");
    ClassAccess     access(storage, id);
    Class& cl = scopeClassCloseV(currentScope.back(), access, moveAccess(DeclListAccess(storage, listId)));
    return storage.add(ClassRef{cl});
}
Class& Action::scopeClassCloseV(Scope& top, Class& cl, DeclList /*decl*/)
{
    Class*          topClass = dynamic_cast<Class*>(&top);

    if (&cl != topClass)
    {
        throw std::domain_error("Internal Error: Scope Note correctly aligned from Class");
    }
    currentScope.pop_back();
    return cl;
}
// ------------------


ClassId Action::scopeClassAnon()
{
    ScopePrinter scope("scopeClassAnon");
    IdentifierId    anonId  = anonName();
    ClassId         classId = scopeClassOpen(anonId);
    return classId;
}
// ------------------

FunctionId Action::scopeFunctionAdd(IdentifierId id, TypeCListId listId, TypeId returnType)
{
    ScopePrinter scope("scopeFunctionAdd");
    IdentifierAccess    access(storage, id);

    Function& function = scopeFunctionAddV(currentScope.back(), access, moveAccess(TypeCListAccess(storage, listId)), TypeAccess(storage, returnType));
    return storage.add(FunctionRef{function});
}
Function& Action::scopeFunctionAddV(Scope& top, Identifier id, TypeCList list, Type const& returnType)
{
    Function&   function = top.add<Function>(this, std::move(id), std::move(list), returnType);
    return function;
}
// ------------------

CodeBlockId Action::scopeCodeBlockOpen()
{
    ScopePrinter scope("scopeCodeBlockOpen");
    CodeBlock& result = scopeCodeBlockOpenV(currentScope.back());
    return storage.add(CodeBlockRef{result});
}
CodeBlock& Action::scopeCodeBlockOpenV(Scope& top)
{
    CodeBlock& result  = top.add<CodeBlock>(this);
    currentScope.emplace_back(result);
    return result;
}
// ------------------

StatementCodeBlockId Action::scopeCodeBlockClose(CodeBlockId id, StatementListId listId)
{
    ScopePrinter scope("scopeCodeBlockClose");
    CodeBlockAccess      access(storage, id);
    StatementCodeBlock& result = scopeCodeBlockCloseV(currentScope.back(), access, moveAccess(StatementListAccess(storage, listId)));
    return storage.add(StatementCodeBlockRef{result});
}
StatementCodeBlock& Action::scopeCodeBlockCloseV(Scope& top, CodeBlock& codeBlock, StatementList list)
{
    CodeBlock*      topBlock = dynamic_cast<CodeBlock*>(&top);

    if (&codeBlock != topBlock)
    {
        throw std::domain_error("Internal Error: Scope Note correctly aligned from CodeBlock");
    }
    currentScope.pop_back();
    Scope&          newTop = currentScope.back();
    StatementCodeBlock& result   = newTop.add<StatementCodeBlock>(this, codeBlock, std::move(list));
    return result;
}
// ------------------

ObjectId Action::scopeObjectAddVariable(IdentifierId name, TypeId type, ExpressionListId init)
{
    ScopePrinter scope("scopeObjectAddVariable");
    IdentifierAccess        nameAccess(storage, name);
    TypeAccess              typeAccess(storage, type);
    ExpressionListAccess    listAccess(storage, init);
    ObjectVariable& object = scopeObjectAddVariableV(currentScope.back(), moveAccess(nameAccess), typeAccess, moveAccess(listAccess));
    return storage.add<ObjectRef>(object);
}
ObjectVariable& Action::scopeObjectAddVariableV(Scope& top, Identifier name, Type const& type, ExpressionList init)
{
    ObjectVariable& object = top.add<ObjectVariable>(this, std::move(name), type, std::move(init));
    return object;
}
// ------------------

ObjectId Action::scopeObjectAddFunction(IdentifierId name, TypeId type, StatementCodeBlockId code, MemberInitListId init)
{
    ScopePrinter scope("scopeObjectAddFunction");
    IdentifierAccess         nameAccess(storage, name);
    TypeAccess               typeAccess(storage, type);
    StatementCodeBlockAccess codeAccess(storage, code);
    MemberInitListAccess     initAccess(storage, init);
    ObjectFunction& object = scopeObjectAddFunctionV(currentScope.back(), moveAccess(nameAccess), typeAccess, codeAccess, moveAccess(initAccess));
    return storage.add<ObjectRef>(object);
}
ObjectFunction& Action::scopeObjectAddFunctionV(Scope& top, Identifier name, Type const& type, StatementCodeBlock& code, MemberInitList init)
{
    ObjectFunction& object = top.add<ObjectFunction>(this, std::move(name), type, code, std::move(init));
    return object;
}
// ------------------

MemberInitId Action::memberInit(IdentifierId name, ExpressionListId init)
{
    ScopePrinter scope("memberInit");
    IdentifierAccess        nameAccess(storage, name);
    ExpressionListAccess    initAccess(storage, init);
    MemberInit& object = memberInitV(currentScope.back(), moveAccess(nameAccess), moveAccess(initAccess));
    return storage.add<MemberInitRef>(object);
}
MemberInit& Action::memberInitV(Scope& top, Identifier name, ExpressionList init)
{
    MemberInit& object = top.add<MemberInit>(this, std::move(name), std::move(init));
    return object;
}
// ------------------

ObjectId Action::scopeConstructorAdd(TypeCListId listId, MemberInitListId init, StatementCodeBlockId code)
{
    ScopePrinter scope("scopeConstructorAdd");
    IdentifierId    conId       = storage.add<std::string>(std::string("$constructor"));
    IdentifierId    voidId      = storage.add<std::string>(std::string("Void"));
    TypeId          voidType    = getNameFromScopeStack<Type>(voidId);
    FunctionId      funcId      = scopeFunctionAdd(anonName(), listId, voidType);


    return scopeObjectAddFunction(conId, convert<Function, Type>(funcId), code, init);
}

// ------------------

ObjectId Action::scopeDestructorAdd(StatementCodeBlockId code)
{
    ScopePrinter scope("scopeDestructorAdd");
    IdentifierId        desId       = storage.add<std::string>(std::string("$destructor"));
    TypeCListId         listId      = storage.add<TypeCList>(TypeCList{});
    IdentifierId        voidId      = storage.add<std::string>(std::string("Void"));
    TypeId              voidType    = getNameFromScopeStack<Type>(voidId);
    FunctionId          funcId      = scopeFunctionAdd(anonName(), listId, voidType);
    MemberInitListId    initId      = storage.add(MemberInitList{});

    return scopeObjectAddFunction(desId, convert<Function, Type>(funcId), code, initId);
}
// ------------------

IdentifierId Action::identifierCreate()
{
    return identifierCreate(std::string(lexer.lexem()));
}
IdentifierId Action::identifierCreate(std::string ident)
{
    ScopePrinter scope("identifierCreate");
    Identifier identifier = identifierCreateV(currentScope.back(), std::move(ident));
    return storage.add(std::move(identifier));
}
Identifier Action::identifierCreateV(Scope& /*top*/, std::string identifier)
{
    return identifier;
}
// ------------------

ExpressionId Action::expressionLiteralString()
{
    std::string_view    view    = lexer.lexem();
    Identifier          literal = std::string(std::begin(view), std::end(view));
    IdentifierId        id      = storage.add<Identifier>(std::move(literal));
    return addDeclToScope<ExpressionLiteral<std::string>>(id);
}
ExpressionId Action::expressionLiteralInt()
{
    std::string_view    view    = lexer.lexem();
    char const*         begView = std::begin(view);
    char const*         endView = std::end(view);
    Int                 literal = std::strtol(begView, const_cast<char**>(&endView), 10);
    IntId               id      = storage.add<Int>(std::move(literal));
    return addDeclToScope<ExpressionLiteral<Int>>(id);
}

// Action Utility Functions
// ========================
std::string Action::getCurrentScopeFullName() const
{
    std::string result;
    for (auto& scopeRefForward: currentScope)
    {
        auto& scopeForward = scopeRefForward.get();
        result += (scopeForward.declName() + "-");
    }

    return result;
}

Id<std::string> Action::assemblyInit()
{
    std::string_view    view = lexer.lexem();
    std::string         token(std::begin(view), std::end(view));
    return storage.add<std::string>(std::move(token));
}

void Action::assembleyAppend(Id<std::string> id)
{
    std::string&        assembley = storage.get<std::string>(id.value);
    std::string_view    view = lexer.lexem();
    assembley.insert(std::begin(assembley), std::begin(view), std::end(view));
}

namespace ThorsAnvil::Anvil::Ice
{

std::ostream& operator<<(std::ostream& str, Action const& action)
{
    str << "Lexer State:\n"
        << "============\n"
        << "Last Token: >" << action.lexer.lexem() << "<\n"
        << "Line:  " << action.lineNo << " : " << action.currentLine.size() << "     FileOffset: " << action.offset << "\n"
        << action.currentLine << "\n"
        << "\n"
        << "Parser State:\n"
        << "=============\n"
        << "\tOpen Scopes Searchable\n"
        << "\t----------------------\n";

    for (auto const& scopeRef: make_View<Forward>(action.currentScope))
    {
        str << "\t" << scopeRef.get().declName() << "\n";
    }
    return str;
}

}

// TODO FIX UP
void Action::invalidCharacter()                                 {addToLine();error("Invalid Character");}
void Action::ignoreSpace()                                      {addToLine();}
void Action::newLine()                                          {resetLine();}

StatementId Action::statmentExpression(ExpressionId id)                             {return addDeclToScope<StatementExpression>(id);}
StatementId Action::statmentReturn(ExpressionId id)                                 {return addDeclToScope<StatementReturn>(id);}
StatementId Action::statmentAssembley(Id<std::string> id)                           {return addDeclToScope<StatementAssembley>(id);}

// Expression:
ExpressionId Action::expressionAssign(ExpressionId, ExpressionId)                        {error("Not Implemented: expressionAssign");}
ExpressionId Action::expressionAssignMul(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignMul");}
ExpressionId Action::expressionAssignDiv(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignDiv");}
ExpressionId Action::expressionAssignMod(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignMod");}
ExpressionId Action::expressionAssignAdd(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignAdd");}
ExpressionId Action::expressionAssignSub(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignSub");}
ExpressionId Action::expressionAssignLSh(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignLSh");}
ExpressionId Action::expressionAssignRSh(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignRSh");}
ExpressionId Action::expressionAssignAnd(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignAnd");}
ExpressionId Action::expressionAssignXOR(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignXOR");}
ExpressionId Action::expressionAssignOR(ExpressionId, ExpressionId)                      {error("Not Implemented: expressionAssignOR");}
ExpressionId Action::expressionConditional(ExpressionId, ExpressionId, ExpressionId)     {error("Not Implemented: expressionConditional");}
ExpressionId Action::expressionLogicalOr(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionLogicalOr");}
ExpressionId Action::expressionLogicalAnd(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionLogicalAnd");}
ExpressionId Action::expressionInclusiveOr(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionInclusiveOr");}
ExpressionId Action::expressionExclusiveOr(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionExclusiveOr");}
ExpressionId Action::expressionAnd(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionAnd");}
ExpressionId Action::expressionEqual(ExpressionId, ExpressionId)                         {error("Not Implemented: expressionEqual");}
ExpressionId Action::expressionNotEqual(ExpressionId, ExpressionId)                      {error("Not Implemented: expressionNotEqual");}
ExpressionId Action::expressionLess(ExpressionId, ExpressionId)                          {error("Not Implemented: expressionLess");}
ExpressionId Action::expressionGreat(ExpressionId, ExpressionId)                         {error("Not Implemented: expressionGreat");}
ExpressionId Action::expressionLessEqual(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionLessEqual");}
ExpressionId Action::expressionGreatEqual(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionGreatEqual");}
ExpressionId Action::expressionShiftLeft(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionShiftLeft");}
ExpressionId Action::expressionShiftRight(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionShiftRight");}
ExpressionId Action::expressionAdd(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionAdd");}
ExpressionId Action::expressionSub(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionSub");}
ExpressionId Action::expressionMul(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionMul");}
ExpressionId Action::expressionDiv(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionDiv");}
ExpressionId Action::expressionMod(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionMod");}
ExpressionId Action::expressionPreInc(ExpressionId)                                      {error("Not Implemented: expressionPreInc");}
ExpressionId Action::expressionPreDec(ExpressionId)                                      {error("Not Implemented: expressionPreDec");}
ExpressionId Action::expressionPlus(ExpressionId)                                        {error("Not Implemented: expressionPlus");}
ExpressionId Action::expressionNeg(ExpressionId)                                         {error("Not Implemented: expressionNeg");}
ExpressionId Action::expressionOneCompliment(ExpressionId)                               {error("Not Implemented: expressionOneCompliment");}
ExpressionId Action::expressionNot(ExpressionId)                                         {error("Not Implemented: expressionNot");}
ExpressionId Action::expressionArrayAccess(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionArrayAccess");}
ExpressionId Action::expressionFuncCall(ExpressionId id, ExpressionListId list)      {return addDeclToScope<ExpressionFuncCall>(id, list);}
ExpressionId Action::expressionMemberAccess(ExpressionId id, IdentifierId mem)       {return addDeclToScope<ExpressionMemberAccess>(id, mem);}
ExpressionId Action::expressionPostInc(ExpressionId)                                     {error("Not Implemented: expressionPostInc");}
ExpressionId Action::expressionPostDec(ExpressionId)                                     {error("Not Implemented: expressionPostDec");}
ExpressionId Action::expressionObject(ObjectId id)                                   {return addDeclToScope<ExpressionObject>(id);}
