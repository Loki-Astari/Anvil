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
    Namespace& updatedNS = Action::scopeNamespaceCloseV(top, ns, decl);
    addDefaultMethodsToScope(updatedNS, std::move(decl));
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
    Class&  updatedClass = Action::scopeClassCloseV(top, cl, decl);
    addDefaultMethodsToScope(updatedClass, std::move(decl));
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

ObjectFunction& Semantic::scopeObjectAddFunctionV(Scope& top, Identifier name, Type const& type, StatementCodeBlock& code, MemberInitList init)
{
    ObjectOverload& objectOverload = getOrAddDeclToScope<ObjectOverload>(top, name);
    Overload const& overload = dynamic_cast<Overload const&>(objectOverload.getType());

    Function const& funcType = dynamic_cast<Function const&>(type);
    bool found = overload.findMatch(this, funcType.getParams());
    if (found)
    {
        error("Function Already defined: ", name);
    }

    ObjectFunction& func = Action::scopeObjectAddFunctionV(top, std::move(name), type, code, std::move(init));

    objectOverload.addOverload(func);
    return func;
}

void Semantic::addDefaultMethodsToScope(Scope& scope, DeclList declList)
{
    std::cerr << "addDefaultMethodsToScope: " << std::size(declList) << "\n";
    auto findCons = scope.get("$constructor");
    if (!findCons.first)
    {
        // TODO: Need to add constructors and destructors for members.
        CodeBlock&          codeBlock       = scope.add<CodeBlock>(this);
        StatementCodeBlock& initCodeInit    = scope.add<StatementCodeBlock>(this, codeBlock, StatementList{});
        Function& constructorType = scope.add<Function>(this, "", TypeCList{}, getRefFromScope<Type>(getGlobalScope(), "Void"));
        scopeObjectAddFunctionV(scope, "$constructor", constructorType, initCodeInit, MemberInitList{});
    }
    auto findDest = scope.get("$destructor");
    if (!findDest.first)
    {
        // TODO: Need to add constructors and destructors for members.
        CodeBlock&          codeBlock       = scope.add<CodeBlock>(this);
        StatementCodeBlock& deinitCodeInit  = scope.add<StatementCodeBlock>(this, codeBlock, StatementList{});
        Function& destructorType = scope.add<Function>(this, "", TypeCList{}, getRefFromScope<Type>(getGlobalScope(), "Void"));
        scopeObjectAddFunctionV(scope, "$destructor", destructorType, deinitCodeInit, MemberInitList{});
    }

    MemberList data;
    std::cerr << "Build Members\n";
    for (auto& decl: declList)
    {
        std::cerr << "Try\n";
        if (decl.get().storageSize() != 0)
        {
            std::cerr << "Add\n";
            Object& var = dynamic_cast<Object&>(decl.get());
            data.emplace_back(var);
        }
    }
    std::cerr << "Build Members: Done\n";

    auto conFind = scope.get("$constructor");
    ObjectOverload&  constructorList = dynamic_cast<ObjectOverload&>(*conFind.second->second);
    for (auto& function: constructorList)
    {
        ObjectFunction&  constructor = dynamic_cast<ObjectFunction&>(function);
        constructor.addMissingMemberInit(this, scope, data, true);
    }

    auto desFind = scope.get("$destructor");
    ObjectOverload&  destructorList = dynamic_cast<ObjectOverload&>(*desFind.second->second);
    for (auto& function: destructorList)
    {
        ObjectFunction&  destructor = dynamic_cast<ObjectFunction&>(function);
        destructor.addMissingMemberInit(this, scope, data, false);
    }
}
