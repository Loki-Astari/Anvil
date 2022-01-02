#include "Storage.h"
#include "Declaration.h"
#include "Common.h"

using namespace ThorsAnvil::Anvil::Ice;

Storage::Storage()
    : nextFree(0)
{
    static Namespace        defaultDecl(ActionRef{}, "Invalid Decl");
    static CodeBlock        defaultCodeBlock(ActionRef{});
    static MemberInit       defaultMemberInit(ActionRef{}, "Invalid Member", ExpressionList{});
    static Namespace        defaultNamespace(ActionRef{}, "Invalid Namespace");
    //static Void             defaultType(ActionRef{});
    //static Void             defaultVoid(ActionRef{});
    static Class            defaultClass(ActionRef{}, "Invalid Class");
    static Function         defaultFunction(ActionRef{}, "Invalid Function", TypeCList{}, Void::getInstance());
    static ObjectVariable   defaultObject(ActionRef{}, "Invalid Object", defaultClass, ExpressionList{});
    static Statement        defaultStatement(ActionRef{});
    static ExpressionObject defaultExpression(ActionRef{}, defaultObject);

    // We don't use location 0.
    // This is because ice.y parser passes back zero for no object returned.
    // So we don't want to confuse a no-object with an object.
    data.emplace_back(0UL);
    // ---
    data.emplace_back(Int{0});
    // ---
    data.emplace_back(Identifier{"Invalid Identifier"});
    // ----
    data.emplace_back(DeclList{});
    data.emplace_back(NamespaceList{});
    data.emplace_back(MemberInitList{});
    data.emplace_back(TypeCList{});
    data.emplace_back(StatementList{});
    data.emplace_back(ExpressionList{});
    // ----
    data.emplace_back(DeclRef{defaultDecl});
    data.emplace_back(CodeBlockRef{defaultCodeBlock});
    data.emplace_back(MemberInitRef{defaultMemberInit});
    data.emplace_back(NamespaceRef{defaultNamespace});
    data.emplace_back(Int(0));//dynamic_cast<Type&>(Void::getInstance()));    // Type
    data.emplace_back(Int(0));//Void::getInstance());                         // Void
    data.emplace_back(ClassRef{defaultClass});
    data.emplace_back(FunctionRef{defaultFunction});
    data.emplace_back(ObjectRef{defaultObject});
    data.emplace_back(StatementRef{defaultStatement});
    data.emplace_back(ExpressionRef{defaultExpression}); // 18
}

void Storage::release(Int index)
{
    if (index == 0)
    {
        throw std::runtime_error("Internal Compiler Error: Releasing Element Zero");
    }
    if (static_cast<std::size_t>(index) >= data.size())
    {
        throw std::runtime_error("Internal Compiler Error: Releasing Element beyond Range");
    }
    data[index] = nextFree;
    nextFree = index;
}
