#include "Semantic.h"
#include "Storage.h"
#include "Declaration.h"

using namespace ThorsAnvil::Anvil::Ice;

Semantic::Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output)
    : Action(lexer, globalScope, storage, output)
    , namespaceDecOrder(namespaceDecOrder)
{}

Semantic::~Semantic()
{
}

// Action Override
Namespace& Semantic::scopeNamespaceOpenV(Scope& top, std::string namespaceName)
{
    Namespace& ns = Action::scopeNamespaceOpenV(top, std::move(namespaceName));
    namespaceDecOrder.emplace_back(ns);
    ns.setPath(getCurrentScopeFullName());

    return ns;
}

Namespace& Semantic::scopeNamespaceCloseV(Scope& top, Namespace& ns, DeclList decl)
{
    addDefaultMethodsToScope(top, decl);
    Namespace& updatedNS = Action::scopeNamespaceCloseV(top, ns, std::move(decl));
    return updatedNS;
}

Class& Semantic::scopeClassOpenV(Scope& top, Identifier className)
{
    Class&  cl = Action::scopeClassOpenV(top, std::move(className));
    cl.setPath(getCurrentScopeFullName());

    return cl;
}

Class& Semantic::scopeClassCloseV(Scope& top, Class& cl, DeclList decl)
{
    addDefaultMethodsToScope(top, decl);
    Class&  updatedClass = Action::scopeClassCloseV(top, cl, std::move(decl));
    return updatedClass;
}

ObjectVariable& Semantic::scopeObjectAddVariableV(Scope& top, Identifier name, Type const& type, ExpressionList init)
{
    auto find = top.get(name);
    if (find.first)
    {
        error("Object Already defined: ", name);
    }

    ObjectVariable& object = Action::scopeObjectAddVariableV(top, std::move(name), type, std::move(init));

    auto constructor = type.get("$constructor");
    if (!constructor.first)
    {
        error("Object can't find constructor");
    }

    return object;
}

ObjectOverload& Semantic::scopeAddFunctionOverload(Scope& top, Identifier name, Type const& type)
{
    ObjectOverload& objectOverload = getOrAddDeclToScope<ObjectOverload>(top, name);
    Overload const& overload = dynamic_cast<Overload const&>(objectOverload.getType());

    Function const& funcType = dynamic_cast<Function const&>(type);
    bool found = overload.findMatch(this, funcType.getParams());
    if (found)
    {
        error("Function Already defined: ", name);
    }

    return objectOverload;
}

ObjectFunction& Semantic::scopeFunctionOpenV(Scope& top, Identifier name, Type const& typeId)
{
    ObjectOverload&  overload   = scopeAddFunctionOverload(top, name, typeId);
    ObjectFunction&  func       = Action::scopeFunctionOpenV(top, std::move(name), typeId);

    overload.addOverload(func);
    func.setPath(getCurrentScopeFullName());
    return func;
}

ObjectFunction& Semantic::scopeFunctionCloseV(Scope& top, ObjectFunction& func, StatementCodeBlock& code, MemberInitList init)
{
    ObjectFunction&  updatedFunc = Action::scopeFunctionCloseV(top, func, code, std::move(init));
    return updatedFunc;
}

void Semantic::addDefaultMethodsToScope(Scope& scope, DeclList& declList)
{
    auto findCons = scope.get("$constructor");
    if (!findCons.first)
    {
        // TODO: Need to add constructors and destructors for members.
        Function&           constructorType = scope.add<Function>(this, "", TypeCList{}, getRefFromScope<Type>(getGlobalScope(), "Void"));
        ObjectFunction&     cons1           = scopeFunctionOpenV(scope, "$constructor", constructorType);
        CodeBlock&          codeBlock       = cons1.add<CodeBlock>(this);
        StatementCodeBlock& initCodeInit    = cons1.add<StatementCodeBlock>(this, codeBlock, StatementList{});
        ObjectFunction&     cons2           = scopeFunctionCloseV(cons1, cons1, initCodeInit, MemberInitList{});
        declList.emplace_back(cons2);
    }
    auto findDest = scope.get("$destructor");
    if (!findDest.first)
    {
        // TODO: Need to add constructors and destructors for members.
        Function&           destructorType  = scope.add<Function>(this, "", TypeCList{}, getRefFromScope<Type>(getGlobalScope(), "Void"));
        ObjectFunction&     des1            = scopeFunctionOpenV(scope, "$destructor", destructorType);
        CodeBlock&          codeBlock       = des1.add<CodeBlock>(this);
        StatementCodeBlock& deinitCodeInit  = des1.add<StatementCodeBlock>(this, codeBlock, StatementList{});
        ObjectFunction&     des2            = scopeFunctionCloseV(des1, des1, deinitCodeInit, MemberInitList{});
        declList.emplace_back(des2);
    }

    MemberList data;
    for (auto& decl: declList)
    {
        if (decl.get().storageSize() != 0)
        {
            Object& var = dynamic_cast<Object&>(decl.get());
            data.emplace_back(var);
        }
    }

    auto conFind = scope.get("$constructor");
    ObjectOverload&  constructorList = dynamic_cast<ObjectOverload&>(*conFind.second->second);
    for (auto& function: constructorList)
    {
        ObjectFunction&  constructor = dynamic_cast<ObjectFunction&>(function);
        constructor.addMissingMemberInit(this, function, data, true);
    }

    auto desFind = scope.get("$destructor");
    ObjectOverload&  destructorList = dynamic_cast<ObjectOverload&>(*desFind.second->second);
    for (auto& function: destructorList)
    {
        ObjectFunction&  destructor = dynamic_cast<ObjectFunction&>(function);
        destructor.addMissingMemberInit(this, function, data, false);
    }
}
