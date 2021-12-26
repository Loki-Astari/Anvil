#include "Action.h"
#include "Lexer.h"
#include "Storage.h"
#include "ParserTypes.h"
#include "Utility/View.h"

#include <cstdlib>

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
std::string Action::anonName()
{
    Scope&          topScope = currentScope.back();
    return topScope.anonName();
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
    auto findCons = scope.get("$Constructor");
    if (!findCons.first)
    {
        // TODO: Need to add constructors and destructors for members.
        static Statement initCodeInit(this);
        Function& constructorType = scope.add<Function>(*this, "$Constructor");
        scope.add<ObjectFunctionConstructor>(*this, "$constructor", constructorType, MemberInitList{}, initCodeInit);
        constructorType.addOverload(this, TypeCList{}, getRefFromScope<Type>(currentScope.front(), "Void"));
    }
    auto findDest = scope.get("$Destructor");
    if (!findDest.first)
    {
        // TODO: Need to add constructors and destructors for members.
        static Statement initCodeInit(this);
        Function& constructorType = scope.add<Function>(*this, "$Destructor");
        scope.add<ObjectFunction>(*this, "$destructor", constructorType, initCodeInit);
        constructorType.addOverload(this, TypeCList{}, getRefFromScope<Type>(currentScope.front(), "Void"));
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
    std::string     anonN   = anonName();
    IdentifierId    anonId  = storage.add<std::string>(std::move(anonN));
    ClassId         classId = scopeClassOpen(anonId);
    return classId;
}
// ------------------

FunctionId Action::scopeFunctionOpen(IdentifierId id)
{
    ScopePrinter scope("scopeFunctionOpen");
    IdentifierAccess    access(storage, id);
    return scopeFunctionOpenV(moveAccess(access), [&access](){return access.reuse();});
}
FunctionId Action::scopeFunctionOpenV(Identifier funcName, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Function&       newFunction = getOrAddScope<Function>(topScope, std::move(funcName));
    currentScope.emplace_back(newFunction);

    return storage.add(FunctionRef{newFunction});
}
// ------------------

FunctionId Action::scopeFunctionClose(FunctionId id, TypeCListId listId, TypeId returnType)
{
    ScopePrinter scope("scopeFunctionClose");
    FunctionAccess      access(storage, id);
    return scopeFunctionCloseV(access, moveAccess(TypeCListAccess(storage, listId)), TypeAccess(storage, returnType), [&access](){return access.reuse();});
}
FunctionId Action::scopeFunctionCloseV(Function& cl, TypeCList list, Type const& returnType, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Function*       topFunction = dynamic_cast<Function*>(&topScope);

    if (&cl != topFunction)
    {
        error("Internal Error: Scope Note correctly aligned from Function");
    }
    topFunction->addOverload(this, std::move(list), returnType);
    currentScope.pop_back();
    return reuse();
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

FunctionId Action::scopeFunctionAnon()
{
    ScopePrinter scope("scopeFunctionAnon");
    std::string     anonN   = anonName();
    IdentifierId    anonId  = storage.add<std::string>(std::move(anonN));
    FunctionId      funcId  = scopeFunctionOpen(anonId);
    return funcId;
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

    auto constructor = type.get("$Constructor");
    if (!constructor.first)
    {
        error("Object can't find constructor");
    }
    Decl& decl = *constructor.second->second;
    Function& func = dynamic_cast<Function&>(decl);

    func.getReturnType(this, init);                     // calls error() if no constructor

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

FunctionId Action::scopeConstructorInit()
{
    ScopePrinter scope("scopeConstructorInit");
    IdentifierId name = storage.add<std::string>(std::string("$Constructor"));
    return scopeFunctionOpen(name);
}
ObjectId Action::scopeConstructorAdd(FunctionId id, TypeCListId listId, MemberInitListId init, StatementId code)
{
    IdentifierId    voidId      = storage.add<std::string>(std::string("Void"));
    TypeId          voidType    = getNameFromScopeStack<Type>(voidId);
    scopeFunctionClose(id, listId, voidType);

    Scope&                  topScope = currentScope.back();
    MemberInitListAccess    initAccess(storage, init);
    StatementAccess         codeAccess(storage, code);
    Type&                   funcType    = getRefFromScopeStack<Type>("$Constructor");
    ObjectFunction& object = topScope.add<ObjectFunctionConstructor>(*this, "$constructor", funcType, initAccess, codeAccess);
    return storage.add<ObjectRef>(object);
}

// ------------------

FunctionId Action::scopeDestructorInit()
{
    ScopePrinter scope("scopeDestructorInit");
    IdentifierId name = storage.add<std::string>(std::string("$Destructor"));
    return scopeFunctionOpen(name);
}
ObjectId Action::scopeDestructorAdd(FunctionId id, TypeCListId listId, StatementId code)
{
    IdentifierId    voidId      = storage.add<std::string>(std::string("Void"));
    TypeId          voidType    = getNameFromScopeStack<Type>(voidId);
    scopeFunctionClose(id, listId, voidType);

    Scope&          topScope = currentScope.back();
    StatementAccess codeAccess(storage, code);
    Type&           funcType    = getRefFromScopeStack<Type>("$Destructor");
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

// Expression Functions
// ========================
#if 0
ExpressionId Action::expressionAssign(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignMul(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignDiv(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignMod(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignAdd(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignSub(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignLSh(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignRSh(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignAnd(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignXOR(ExpressionId, ExpressionId);
ExpressionId Action::expressionAssignOR(ExpressionId, ExpressionId);
ExpressionId Action::expressionConditional(ExpressionId, ExpressionId, ExpressionId);
ExpressionId Action::expressionLogicalOr(ExpressionId, ExpressionId);
ExpressionId Action::expressionLogicalAnd(ExpressionId, ExpressionId);
ExpressionId Action::expressionInclusiveOr(ExpressionId, ExpressionId);
ExpressionId Action::expressionExclusiveOr(ExpressionId, ExpressionId);
ExpressionId Action::expressionAnd(ExpressionId, ExpressionId);
ExpressionId Action::expressionEqual(ExpressionId, ExpressionId);
ExpressionId Action::expressionNotEqual(ExpressionId, ExpressionId);
ExpressionId Action::expressionLess(ExpressionId, ExpressionId);
ExpressionId Action::expressionGreat(ExpressionId, ExpressionId);
ExpressionId Action::expressionLessEqual(ExpressionId, ExpressionId);
ExpressionId Action::expressionGreatEqual(ExpressionId, ExpressionId);
ExpressionId Action::expressionShiftLeft(ExpressionId, ExpressionId);
ExpressionId Action::expressionShiftRight(ExpressionId, ExpressionId);
ExpressionId Action::expressionAdd(ExpressionId, ExpressionId);
ExpressionId Action::expressionSub(ExpressionId, ExpressionId);
ExpressionId Action::expressionMul(ExpressionId, ExpressionId);
ExpressionId Action::expressionDiv(ExpressionId, ExpressionId);
ExpressionId Action::expressionMod(ExpressionId, ExpressionId);
ExpressionId Action::expressionPreInc(ExpressionId);
ExpressionId Action::expressionPreDec(ExpressionId);
ExpressionId Action::expressionPlus(ExpressionId);
ExpressionId Action::expressionNeg(ExpressionId);
ExpressionId Action::expressionOneCompliment(ExpressionId);
ExpressionId Action::expressionNot(ExpressionId);
ExpressionId Action::expressionArrayAccess(ExpressionId, ExpressionId);
ExpressionId Action::expressionFuncCall(ExpressionId, ExpressionId);
ExpressionId Action::expressionMemberAccess(ExpressionId, ExpressionId);
ExpressionId Action::expressionPostInc(ExpressionId);
ExpressionId Action::expressionPostDec(ExpressionId);
#endif
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
template<typename T>
T& Action::getOrAddScope(Scope& topScope, std::string scopeName)
{
    auto find = topScope.get(scopeName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        return dynamic_cast<T&>(*foundDeclWithSameName);
    }
    T& addedScope = topScope.add<T>(*this, std::move(scopeName));
    addedScope.setPath(getCurrentScopeFullName());
    return addedScope;
}

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
