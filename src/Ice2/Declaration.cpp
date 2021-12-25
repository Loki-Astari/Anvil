#include "Declaration.h"
#include "ParserTypes.h"
#include "Action.h"

using namespace ThorsAnvil::Anvil::Ice;

#pragma vera-pushoff
std::string const ExpressionLiteralTypeName<std::string>::standardName = "String";
std::string const ExpressionLiteralTypeName<Int>::standardName = "Integer";
#pragma vera-pop


std::string const& Decl::declName() const
{
    static std::string emptyName;  // using an empty name forces the scope to generate
                                   // an anonymous name for the object. See: Scope::add
    return emptyName;
}

Scope::Scope(ActionRef action)
    : Decl(action)
    , nextObjectId(0)
    , anonNameCount(0)
{}

std::pair<bool, NameRef> Scope::get(std::string const& name) const
{
    auto find = members.find(name);
    if (find == members.end())
    {
        return {false, find};
    }
    return {true, find};
}

namespace ThorsAnvil::Anvil::Ice
{
    std::ostream& operator<<(std::ostream& str, Scope& data)
    {
        str << "\n\n\n===================\n";
        for (auto const& member: data.members)
        {
            str << member.first << " : " << member.second->declName() << "\n";
        }
        str << "===================\n\n\n";
        return str;
    }
}

std::string Scope::anonName()
{
    // 64 bit Size: Convert to hex => 16 bit
    // Prefix with dollar => 17bit
    std::string name(17, '$');
    generateHexName(name, anonNameCount);
    ++anonNameCount;
    return name;
}

char Scope::hex(std::size_t halfByte)
{
    return halfByte < 10
                ? '0' + halfByte
                : 'a' + (halfByte - 10);
}

void Scope::generateHexName(std::string& name, std::size_t count)
{
    for (int loop = 0; loop < 16; ++loop)
    {
        name[loop+1] = hex((count >> (loop * 4)) & 0xF);
    }
}

Void::Void(ActionRef action)
    : Type(action, "Void")
{
    static Statement voidCodeInit(action);
    Function& constructorType = add<Function>(*action, "$Constructor");
    add<ObjectFunction>(*action, "$constructor", constructorType, voidCodeInit);
    constructorType.addOverload(action, TypeList{}, *this);
}

Object& ExpressionMemberAccess::findMember(ActionRef action, Identifier const& memberName)
{
    Type const& type = src.getType();
    auto find = type.get(memberName);
    if (!find.first)
    {
        action->error("Invalid Member Name: ", memberName);
    }
    Decl&   decl = *find.second->second;
    Object& object = dynamic_cast<Object&>(decl);
    return object;
}

Type const& ExpressionFuncCall::findType(ActionRef action)
{
    Type const& funcType = funcObject.getType();
    if (funcType.declType() != DeclType::Function)
    {
        action->error("Makeing a function call with a non function object");
    }
    Function const& functionInfo = dynamic_cast<Function const&>(funcType);
    return functionInfo.getReturnType(action, params);
}

void Function::addOverload(ActionRef action, TypeList list, Type const& returnType)
{
    auto find = overload.find(list);
    if (find != overload.end())
    {
        action->error("Function Overload Error: This parameter list already exists");
    }
    overload.emplace(std::move(list), returnType);
}

Type const& Function::getReturnType(ActionRef action, ExpressionList const& params) const
{
    TypeList    typeInfo;
    for (auto const& expr: params)
    {
        typeInfo.emplace_back(const_cast<Type&>(expr.get().getType()));
    }
    auto find = overload.find(typeInfo);
    if (find == overload.end())
    {
        action->error("Function Overload Missing: No function exists that takes these parameters");
    }
    return find->second;
}

template<typename T>
Type const& ExpressionLiteral<T>::findType(ActionRef action)
{
    Namespace& global = action->getRefFromScopeStack<Namespace>("::");
    Namespace& stdns  = action->getRefFromScope<Namespace>(global, "Std");
    Type&      string = action->getRefFromScope<Type>(stdns, ExpressionLiteralTypeName<T>::standardName);
    return string;
}

namespace ThorsAnvil::Anvil::Ice
{
    template class ExpressionLiteral<std::string>;
    template class ExpressionLiteral<Int>;
}
