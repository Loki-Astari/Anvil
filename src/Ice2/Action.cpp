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
    globalScope.add<Void>(*this);
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
    return anvilProgramV(moveAccess(access), [&access](){return access.reuse();});
}
VoidId Action::anvilProgramV(NamespaceList /*list*/, Reuse&& /*reuse*/)
{
    return 0;
}
// ------------------

NamespaceId Action::scopeNamespaceOpen(IdentifierId id)
{
    ScopePrinter scope("scopeNamespaceOpen");
    IdentifierAccess    access(storage, id);
    return scopeNamespaceOpenV(moveAccess(access), [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceOpenV(Identifier namespaceName, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Namespace&      newNameSpace = getOrAddScope<Namespace>(topScope, std::move(namespaceName));
    currentScope.emplace_back(newNameSpace);

    return storage.add(NamespaceRef{newNameSpace});
}
// ------------------

NamespaceId Action::scopeNamespaceClose(NamespaceId id, DeclListId listId)
{
    ScopePrinter scope("scopeNamespaceClose");
    NamespaceAccess     access(storage, id);
    return scopeNamespaceCloseV(access, moveAccess(DeclListAccess(storage, listId)), [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceCloseV(Namespace& ns, DeclList decl, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Namespace*      topNS = dynamic_cast<Namespace*>(&topScope);

    if (&ns != topNS)
    {
        error("Internal Error: Scope Note correctly aligned from Namespace");
    }
    addDefaultMethodsToScope(ns, std::move(decl));
    currentScope.pop_back();
    return reuse();;
}
// ------------------

ClassId Action::scopeClassOpen(IdentifierId id)
{
    ScopePrinter scope("scopeClassOpen");
    IdentifierAccess     access(storage, id);
    return scopeClassOpenV(moveAccess(access), [&access](){return access.reuse();});
}
ClassId Action::scopeClassOpenV(Identifier className, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Class&          newClass = getOrAddScope<Class>(topScope, std::move(className));
    currentScope.emplace_back(newClass);

    return storage.add(ClassRef{newClass});
}
// ------------------

ClassId Action::scopeClassClose(ClassId id, DeclListId listId)
{
    ScopePrinter scope("scopeClassClose");
    ClassAccess     access(storage, id);
    return scopeClassCloseV(access, moveAccess(DeclListAccess(storage, listId)), [&access](){return access.reuse();});
}
ClassId Action::scopeClassCloseV(Class& cl, DeclList decl, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Class*          topClass = dynamic_cast<Class*>(&topScope);

    if (&cl != topClass)
    {
        error("Internal Error: Scope Note correctly aligned from Class");
    }
    addDefaultMethodsToScope(cl, std::move(decl));
    currentScope.pop_back();
    return reuse();
}
// ------------------

void Action::addDefaultMethodsToScope(Scope& scope, DeclList declList)
{
    auto findCons = scope.get("$constructor");
    if (!findCons.first)
    {
        // TODO: Need to add constructors and destructors for members.
        static Statement initCodeInit(this);
        Function& constructorType = scope.add<Function>(*this, "", TypeCList{}, getRefFromScope<Type>(currentScope.front(), "Void"));
        scope.add<ObjectFunctionConstructor>(*this, "$constructor", constructorType, MemberInitList{}, initCodeInit);
    }
    auto findDest = scope.get("$destructor");
    if (!findDest.first)
    {
        // TODO: Need to add constructors and destructors for members.
        static Statement initCodeInit(this);
        Function& constructorType = scope.add<Function>(*this, "", TypeCList{}, getRefFromScope<Type>(currentScope.front(), "Void"));
        scope.add<ObjectFunction>(*this, "$destructor", constructorType, initCodeInit);
    }

    std::vector<ObjectVariableCRef>  data;
    for (auto const& decl: declList)
    {
        if (decl.get().declType() == DeclType::ObjectVariable)
        {
            ObjectVariable const& var = dynamic_cast<ObjectVariable const&>(decl.get());
            data.emplace_back(var);

            //std::cerr << "Name: " << var.declName() << "\n";
        }
    }
    //std::cerr << "\n-------------------\n\n";
    ((void)data);
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

    return scopeFunctionAddV(access, moveAccess(TypeCListAccess(storage, listId)), TypeAccess(storage, returnType), [&access](){return access.reuse();});
}
FunctionId Action::scopeFunctionAddV(Identifier& id, TypeCList list, Type const& returnType, Reuse&& /*reuse*/)
{
    Scope&      topScope = currentScope.back();
    Function&   function = topScope.add<Function>(*this, std::move(id), std::move(list), returnType);

    return storage.add(FunctionRef{function});
}
// ------------------

CodeBlockId Action::scopeCodeBlockOpen()
{
    ScopePrinter scope("scopeCodeBlockOpen");
    return scopeCodeBlockOpenV();
}
CodeBlockId Action::scopeCodeBlockOpenV()
{
    Scope&     topScope = currentScope.back();
    CodeBlock& result  = topScope.add<CodeBlock>(*this);
    currentScope.emplace_back(result);

    return storage.add(CodeBlockRef{result});
}
// ------------------

StatementId Action::scopeCodeBlockClose(CodeBlockId id, StatementListId listId)
{
    ScopePrinter scope("scopeCodeBlockClose");
    CodeBlockAccess      access(storage, id);
    return scopeCodeBlockCloseV(access, moveAccess(StatementListAccess(storage, listId)));
}
StatementId Action::scopeCodeBlockCloseV(CodeBlock& codeBlock, StatementList list)
{
    Scope&          topScope = currentScope.back();
    CodeBlock*      topBlock = dynamic_cast<CodeBlock*>(&topScope);

    if (&codeBlock != topBlock)
    {
        error("Internal Error: Scope Note correctly aligned from CodeBlock");
    }
    currentScope.pop_back();
    Scope&          newTop = currentScope.back();
    StatementCodeBlock& result   = newTop.add<StatementCodeBlock>(*this, codeBlock, std::move(list));
    return storage.add(StatementRef{result});
}
// ------------------

ObjectId Action::scopeObjectAddVariable(IdentifierId name, TypeId type, ExpressionListId init)
{
    ScopePrinter scope("scopeObjectAddVariable");
    IdentifierAccess        nameAccess(storage, name);
    TypeAccess              typeAccess(storage, type);
    ExpressionListAccess    listAccess(storage, init);
    return scopeObjectAddVariableV(moveAccess(nameAccess), typeAccess, moveAccess(listAccess));
}
ObjectId Action::scopeObjectAddVariableV(Identifier name, Type const& type, ExpressionList init)
{
    Scope&  topScope = currentScope.back();
    auto find = topScope.get(name);
    if (find.first)
    {
        error("Object Already defined: ", name);
    }

    auto constructor = type.get("$constructor");
    if (!constructor.first)
    {
        error("Object can't find constructor");
    }

    ObjectVariable& object = topScope.add<ObjectVariable>(*this, std::move(name), type, std::move(init));
    return storage.add<ObjectRef>(object);
}
// ------------------

ObjectId Action::scopeObjectAddFunction(IdentifierId name, TypeId type, StatementId init)
{
    ScopePrinter scope("scopeObjectAddFunction");
    IdentifierAccess        nameAccess(storage, name);
    TypeAccess              typeAccess(storage, type);
    StatementAccess         codeAccess(storage, init);
    return scopeObjectAddFunctionV(moveAccess(nameAccess), typeAccess, codeAccess);
}
ObjectId Action::scopeObjectAddFunctionV(Identifier name, Type const& type, Statement& code)
{
    Scope&  topScope = currentScope.back();
    auto find = topScope.get(name);
    if (find.first)
    {
        error("Object Already defined: ", name);
    }
    ObjectFunction& object = topScope.add<ObjectFunction>(*this, std::move(name), type, code);
    return storage.add<ObjectRef>(object);
}
// ------------------

MemberInitId Action::memberInit(IdentifierId name, ExpressionListId init)
{
    ScopePrinter scope("memberInit");
    IdentifierAccess        nameAccess(storage, name);
    ExpressionListAccess    initAccess(storage, init);
    return memberInitV(moveAccess(nameAccess), moveAccess(initAccess));
}
MemberInitId Action::memberInitV(Identifier name, ExpressionList init)
{
    Scope&  topScope = currentScope.back();
    MemberInit& object = topScope.add<MemberInit>(*this, std::move(name), std::move(init));
    return storage.add<MemberInitRef>(object);
}
// ------------------

ObjectId Action::scopeConstructorAdd(TypeCListId listId, MemberInitListId init, StatementId code)
{
    ScopePrinter scope("scopeConstructorAdd");
    IdentifierId    voidId      = storage.add<std::string>(std::string("Void"));
    TypeId          voidType    = getNameFromScopeStack<Type>(voidId);
    FunctionId      funcId      = scopeFunctionAdd(anonName(), listId, voidType);
    FunctionAccess  funcAccess(storage, funcId);

    Scope&                  topScope = currentScope.back();
    MemberInitListAccess    initAccess(storage, init);
    StatementAccess         codeAccess(storage, code);
    Type&                   funcType    = funcAccess;

    ObjectFunction& object = topScope.add<ObjectFunctionConstructor>(*this, "$constructor", funcType, initAccess, codeAccess);
    return storage.add<ObjectRef>(object);
}

// ------------------

ObjectId Action::scopeDestructorAdd(StatementId code)
{
    ScopePrinter scope("scopeDestructorAdd");
    TypeCListId     listId      = storage.add<TypeCList>(TypeCList{});
    IdentifierId    voidId      = storage.add<std::string>(std::string("Void"));
    TypeId          voidType    = getNameFromScopeStack<Type>(voidId);
    FunctionId      funcId      = scopeFunctionAdd(anonName(), listId, voidType);
    FunctionAccess  funcAccess(storage, funcId);

    Scope&                  topScope = currentScope.back();
    StatementAccess         codeAccess(storage, code);
    Type&                   funcType    = funcAccess;

    ObjectFunction& object = topScope.add<ObjectFunction>(*this, "$destructor", funcType, codeAccess);
    return storage.add<ObjectRef>(object);
}
// ------------------

IdentifierId Action::identifierCreate()
{
    ScopePrinter scope("identifierCreate");
    return identifierCreateV();
}
IdentifierId Action::identifierCreateV()
{
    return storage.add<std::string>(std::string(lexer.lexem()));
}
// ------------------

ExpressionId Action::expressionLiteralString()
{
    std::string_view    view    = lexer.lexem();
    Identifier          literal = std::string(std::begin(view), std::end(view));
    IdentifierId        id      = storage.add<Identifier>(std::move(literal));
    return addObjectToScope1<ExpressionLiteral<std::string>, Identifier>(id);
}
ExpressionId Action::expressionLiteralInt()
{
    std::string_view    view    = lexer.lexem();
    char const*         begView = std::begin(view);
    char const*         endView = std::end(view);
    Int                 literal = std::strtol(begView, const_cast<char**>(&endView), 10);
    IntId               id      = storage.add<Int>(std::move(literal));
    return addObjectToScope1<ExpressionLiteral<Int>, Int>(id);
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

// Storage Access
// ========================
template<typename T>
IdAccess<T>::~IdAccess()
{
    if (index != 0)
    {
        storage.release(index);
    }
}

template<typename T>
IdAccess<T>::operator T&() const
{
    Int location = (index != 0) ? index : IdTraits<T>::defaultStorageId;
    return storage.get<AccessType>(location);
}

namespace ThorsAnvil::Anvil::Ice
{

std::ostream& operator<<(std::ostream& str, Action const& action)
{
    return str
             << "Last Token: >" << action.lexer.lexem() << "<\n"
             << "Line:  " << action.lineNo << " : " << action.currentLine.size() << "     FileOffset: " << action.offset << "\n"
             << action.currentLine << "\n"
             << "\n";
}

template struct IdAccess<Identifier>;
template struct IdAccess<Namespace>;
template struct IdAccess<Decl>;
template struct IdAccess<Scope>;
template struct IdAccess<Type>;
template struct IdAccess<MemberInit>;
template struct IdAccess<Object>;
template struct IdAccess<Statement>;
template struct IdAccess<Expression>;
template struct IdAccess<ExpressionList>;
template struct IdAccess<StatementList>;
template struct IdAccess<MemberInitList>;


}

// TODO FIX UP
void Action::invalidCharacter()                                 {addToLine();error("Invalid Character");}
void Action::ignoreSpace()                                      {addToLine();}
void Action::newLine()                                          {resetLine();}

StatementId Action::statmentExpression(ExpressionId id)                             {return addObjectToScope1<StatementExpression, Expression>(id);}
StatementId Action::statmentReturn(ExpressionId id)                                 {return addObjectToScope1<StatementReturn, Expression>(id);}
StatementId Action::statmentAssembley(Id<std::string> id)                           {return addObjectToScope1<StatementAssembley, std::string>(id);}

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
ExpressionId Action::expressionFuncCall(ExpressionId id, ExpressionListId list)      {return addObjectToScope2<ExpressionFuncCall, Expression, ExpressionList>(id, list);}
ExpressionId Action::expressionMemberAccess(ExpressionId id, IdentifierId mem)       {return addObjectToScope2<ExpressionMemberAccess, Expression, Identifier>(id, mem);}
ExpressionId Action::expressionPostInc(ExpressionId)                                     {error("Not Implemented: expressionPostInc");}
ExpressionId Action::expressionPostDec(ExpressionId)                                     {error("Not Implemented: expressionPostDec");}
ExpressionId Action::expressionObject(ObjectId id)                                   {return addObjectToScope1<ExpressionObject, Object>(id);}
