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
NamespaceId Semantic::scopeNamespaceOpenV(Scope& top, std::string namespaceName, Reuse&& reuse)
{
    NamespaceId         id = Action::scopeNamespaceOpenV(top, std::move(namespaceName), std::move(reuse));
    NamespaceAccess     access(storage, id);
    Namespace&          ns = access;

    namespaceDecOrder.emplace_back(ns);

    ns.setPath(getCurrentScopeFullName());

    return access.reuse();;
}

NamespaceId Semantic::scopeNamespaceCloseV(Scope& top, Namespace& ns, DeclList decl, Reuse&& reuse)
{
    NamespaceId         id = Action::scopeNamespaceCloseV(top, ns, decl, std::move(reuse));
    addDefaultMethodsToScope(ns, std::move(decl));
    return id;
}

ClassId Semantic::scopeClassOpenV(Scope& top, Identifier className, Reuse&& reuse)
{
    ClassId             id = Action::scopeClassOpenV(top, std::move(className), std::move(reuse));
    ClassAccess         access(storage, id);
    Class&              cl = access;

    cl.setPath(getCurrentScopeFullName());

    return access.reuse();
}

ClassId Semantic::scopeClassCloseV(Scope& top, Class& cl, DeclList decl, Reuse&& reuse)
{
    ClassId             id = Action::scopeClassCloseV(top, cl, decl, std::move(reuse));
    addDefaultMethodsToScope(cl, std::move(decl));
    return id;
}

ObjectId Semantic::scopeObjectAddVariableV(Scope& top, Identifier name, Type const& type, ExpressionList init)
{
    auto find = top.get(name);
    if (find.first)
    {
        error("Object Already defined: ", name);
    }

    ObjectId objectId = Action::scopeObjectAddVariableV(top, std::move(name), type, std::move(init));

    auto constructor = type.get("$constructor");
    if (!constructor.first)
    {
        error("Object can't find constructor");
    }

    return objectId;
}

ObjectId Semantic::scopeObjectAddFunctionV(Scope& top, Identifier name, Type const& type, StatementCodeBlock& code, MemberInitList init)
{
    ObjectOverload& objectOverload = getOrAddDeclToScope<ObjectOverload>(top, name);
    Overload const& overload = dynamic_cast<Overload const&>(objectOverload.getType());

    Function const& funcType = dynamic_cast<Function const&>(type);
    bool found = overload.findMatch(this, funcType.getParams());
    if (found)
    {
        error("Function Already defined: ", name);
    }

    ObjectId        objectId = Action::scopeObjectAddFunctionV(top, std::move(name), type, code, std::move(init));
    ObjectAccess    objectAccess(storage, objectId);
    Object&         object = objectAccess;
    ObjectFunction& func = dynamic_cast<ObjectFunction&>(object);

    objectOverload.addOverload(func);
    return objectAccess.reuse();
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
