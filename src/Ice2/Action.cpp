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
    globalScope.add<Void>(*this, "Void");
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
    return anvilProgramV(access, [&access](){return access.reuse();});
}
VoidId Action::anvilProgramV(NamespaceList& /*list*/, Reuse&& /*reuse*/)
{
    return 0;
}
// ------------------

NamespaceId Action::scopeNamespaceOpen(IdentifierId id)
{
    ScopePrinter scope("scopeNamespaceOpen");
    IdentifierAccess    access(storage, id);
    return scopeNamespaceOpenV(access, [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceOpenV(std::string& namespaceName, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Namespace&      newNameSpace = getOrAddScope<Namespace>(topScope, namespaceName);
    currentScope.emplace_back(newNameSpace);

    return storage.add(NamespaceRef{newNameSpace});
}
// ------------------

NamespaceId Action::scopeNamespaceClose(NamespaceId id, DeclListId listId)
{
    ScopePrinter scope("scopeNamespaceClose");
    NamespaceAccess     access(storage, id);
    return scopeNamespaceCloseV(access, DeclListAccess(storage, listId), [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceCloseV(Namespace& ns, DeclList&, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Namespace*      topNS = dynamic_cast<Namespace*>(&topScope);

    if (&ns != topNS)
    {
        error("Internal Error: Scope Note correctly aligned from Namespace");
    }
    currentScope.pop_back();
    return reuse();;
}
// ------------------

ClassId Action::scopeClassOpen(IdentifierId id)
{
    ScopePrinter scope("scopeClassOpen");
    IdentifierAccess     access(storage, id);
    return scopeClassOpenV(access, [&access](){return access.reuse();});
}
ClassId Action::scopeClassOpenV(std::string& className, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Class&          newClass = getOrAddScope<Class>(topScope, className);
    currentScope.emplace_back(newClass);

    return storage.add(ClassRef{newClass});
}
// ------------------

ClassId Action::scopeClassClose(ClassId id, DeclListId listId)
{
    ScopePrinter scope("scopeClassClose");
    ClassAccess     access(storage, id);
    return scopeClassCloseV(access, DeclListAccess(storage, listId), [&access](){return access.reuse();});
}
ClassId Action::scopeClassCloseV(Class& cl, DeclList& /*list*/, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Class*          topClass = dynamic_cast<Class*>(&topScope);

    if (&cl != topClass)
    {
        error("Internal Error: Scope Note correctly aligned from Class");
    }
    currentScope.pop_back();
    return reuse();
}
// ------------------

ClassId Action::scopeClassAnon(DeclListId listId)
{
    ScopePrinter scope("scopeClassAnon");
    std::string     anonN   = anonName();
    IdentifierId    anonId  = storage.add<std::string>(std::move(anonN));
    ClassId         classId = scopeClassOpen(anonId);
    classId = scopeClassClose(classId, listId);
    return classId;
}
// ------------------

FunctionId Action::scopeFunctionOpen(IdentifierId id)
{
    ScopePrinter scope("scopeFunctionOpen");
    IdentifierAccess    access(storage, id);
    return scopeFunctionOpenV(access, [&access](){return access.reuse();});
}
FunctionId Action::scopeFunctionOpenV(std::string& className, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Function&       newFunction = getOrAddScope<Function>(topScope, className);
    currentScope.emplace_back(newFunction);

    return storage.add(FunctionRef{newFunction});
}
// ------------------

FunctionId Action::scopeFunctionClose(FunctionId id, TypeListId listId, TypeId returnType)
{
    ScopePrinter scope("scopeFunctionClose");
    FunctionAccess      access(storage, id);
    return scopeFunctionCloseV(access, TypeListAccess(storage, listId), TypeAccess(storage, returnType), [&access](){return access.reuse();});
}
FunctionId Action::scopeFunctionCloseV(Function& cl, TypeList& list, Type& returnType, Reuse&& reuse)
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

FunctionId Action::scopeFunctionAnon(TypeListId listId, TypeId returnType)
{
    ScopePrinter scope("scopeFunctionAnon");
    std::string     anonN   = anonName();
    IdentifierId    anonId  = storage.add<std::string>(std::move(anonN));
    FunctionId      funcId  = scopeFunctionOpen(anonId);
    funcId = scopeFunctionClose(funcId, listId, returnType);
    return funcId;
}

// ------------------

ObjectId Action::scopeObjectAdd(IdentifierId name, TypeId type, ObjectInitId /*init*/)
{
    ScopePrinter scope("scopeObjectAdd");
    IdentifierAccess    nameAccess(storage, name);
    TypeAccess          typeAccess(storage, type);
    return scopeObjectAddV(nameAccess, typeAccess);
}
ObjectId Action::scopeObjectAddV(Identifier& name, Type& type)
{
    Scope&  topScope = currentScope.back();
    auto find = topScope.get(name);
    if (find.first)
    {
        error("Object Already defined: ", name);
    }
    Object& object = topScope.add<Object>(*this, name, type);
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


ObjectInitId Action::initVariable(ExpressionListId /*listId*/)
{
    return 0;
}
ObjectInitId Action::initFunction(StatementListId /*listId*/)
{
    return 0;
}

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
T& Action::getOrAddScope(Scope& topScope, std::string const& scopeName)
{
    auto find = topScope.get(scopeName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        return dynamic_cast<T&>(*foundDeclWithSameName);
    }
    T& addedScope = topScope.add<T>(*this, scopeName);
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
template struct IdAccess<Object>;
template struct IdAccess<Statement>;
template struct IdAccess<Expression>;
template struct IdAccess<ExpressionList>;
template struct IdAccess<StatementList>;


}
