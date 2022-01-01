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
NamespaceId Semantic::scopeNamespaceOpenV(std::string namespaceName, Reuse&& reuse)
{
    NamespaceId         id = Action::scopeNamespaceOpenV(std::move(namespaceName), std::move(reuse));
    NamespaceAccess     access(storage, id);
    Namespace&          ns = access;

    namespaceDecOrder.emplace_back(ns);

    ns.setPath(getCurrentScopeFullName());

    return access.reuse();;
}

NamespaceId Semantic::scopeNamespaceCloseV(Namespace& ns, DeclList decl, Reuse&& reuse)
{
    NamespaceId         id = Action::scopeNamespaceCloseV(ns, decl, std::move(reuse));
    addDefaultMethodsToScope(ns, std::move(decl));
    return id;
}

ClassId Semantic::scopeClassOpenV(Identifier className, Reuse&& reuse)
{
    ClassId             id = Action::scopeClassOpenV(std::move(className), std::move(reuse));
    ClassAccess         access(storage, id);
    Class&              cl = access;

    cl.setPath(getCurrentScopeFullName());

    return access.reuse();
}

ClassId Semantic::scopeClassCloseV(Class& cl, DeclList decl, Reuse&& reuse)
{
    ClassId             id = Action::scopeClassCloseV(cl, decl, std::move(reuse));
    addDefaultMethodsToScope(cl, std::move(decl));
    return id;
}

ObjectId Semantic::scopeObjectAddVariableV(Identifier name, Type const& type, ExpressionList init)
{
    ObjectId objectId = Action::scopeObjectAddVariableV(std::move(name), type, std::move(init));

    auto constructor = type.get("$constructor");
    if (!constructor.first)
    {
        error("Object can't find constructor");
    }

    return objectId;
}

ObjectId Semantic::scopeObjectAddFunctionV(Identifier name, Type const& type, StatementCodeBlock& code)
{
    Scope&  topScope = getCurrentScope();
    auto find = topScope.get(name);
    if (find.first)
    {
        ObjectOverload& overloadObj = dynamic_cast<ObjectOverload&>(*find.second->second);
        Overload const& overload = dynamic_cast<Overload const&>(overloadObj.getType());

        Function const& funcType = dynamic_cast<Function const&>(type);
        bool found = overload.findMatch(this, funcType.getParams());
        if (found)
        {
            error("Function Already defined: ", name);
        }
    }

    ObjectId objectId = Action::scopeObjectAddFunctionV(std::move(name), type, code);
    return objectId;
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
        addFunctionToScope<ObjectFunctionConstructor>(scope, "$constructor", constructorType, MemberInitList{}, initCodeInit);
    }
    auto findDest = scope.get("$destructor");
    if (!findDest.first)
    {
        // TODO: Need to add constructors and destructors for members.
        CodeBlock&          codeBlock       = scope.add<CodeBlock>(this);
        StatementCodeBlock& deinitCodeInit  = scope.add<StatementCodeBlock>(this, codeBlock, StatementList{});
        Function& destructorType = scope.add<Function>(this, "", TypeCList{}, getRefFromScope<Type>(getGlobalScope(), "Void"));
        addFunctionToScope<ObjectFunctionDestructor>(scope, "$destructor", destructorType, MemberInitList{}, deinitCodeInit);
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
        ObjectFunctionConstructor&  constructor = dynamic_cast<ObjectFunctionConstructor&>(function);
        constructor.addMissingMemberInit(this, scope, data, true);
    }

    auto desFind = scope.get("$destructor");
    ObjectOverload&  destructorList = dynamic_cast<ObjectOverload&>(*desFind.second->second);
    for (auto& function: destructorList)
    {
        ObjectFunctionDestructor&  destructor = dynamic_cast<ObjectFunctionDestructor&>(function);
        destructor.addMissingMemberInit(this, scope, data, false);
    }
}
