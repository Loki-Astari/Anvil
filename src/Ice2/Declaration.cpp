#include "Declaration.h"
#include "StorageAccess.h"
#include "Action.h"

using namespace ThorsAnvil::Anvil::Ice;

#pragma vera-pushoff
std::string const ExpressionLiteralTypeName<std::string>::standardName = "String";
std::string const ExpressionLiteralTypeName<Int>::standardName = "Integer";
#pragma vera-pop

Decl::Decl(ActionRef /*action*/)
{
    // std::cerr << "Creating:   " << this << "\n";
}
Decl::~Decl()
{
    // std::cerr << "Destroying: " << this << "\n";
}

std::string const& Decl::declName() const
{
    static std::string emptyName;  // using an empty name forces the scope to generate
                                   // an anonymous name for the object. See: Scope::add
    return emptyName;
}

bool Decl::needsStorage() const
{
    return false;
}

bool Decl::overloadable() const
{
    return false;
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

Decl& Scope::saveMember(Action& action, std::unique_ptr<Decl>&& member)
{
    Decl& result = *member;

    std::string  index = member->declName();
    if (index == "")
    {
        index = anonName();
    }
    if (member->needsStorage())
    {
        objectId[index] = nextObjectId++;
    }


    auto& location = members[index];
    if (location.get() != nullptr)
    {
        action.error("Location already in use: ", index);
    }
    else
    {
        location = std::move(member);
    }
    return result;
}

Void::Void(ActionRef action)
    : Type(action, "Void")
{
    static Statement voidCodeInit(action);
    Function& constructorType = add<Function>(*action, "", TypeCList{}, *this);
    add<ObjectFunction>(*action, "$constructor", constructorType, voidCodeInit);
}

Type const& Overload::getReturnType(ActionRef action, TypeCList const& index) const
{
    auto find = overloadData.find(index);
    if (find == overloadData.end())
    {
        action->error("Did not find overlaad");
    }
    return find->second.get().getReturnType(action);
}

void Overload::addOverload(Function const& type)
{
    overloadData.emplace(type.getParams(), FunctionCRef{type});
}

void ObjectOverload::addOverload(std::unique_ptr<Decl>&& decl)
{
    std::unique_ptr<ObjectFunction> object(dynamic_cast<ObjectFunction*>(decl.release()));

    Type&       type = const_cast<Type&>(getType());
    Overload&   overload = dynamic_cast<Overload&>(type);

    Type&       objectType = const_cast<Type&>(object->getType());
    Function&   objectFunc = dynamic_cast<Function&>(objectType);

    overload.addOverload(objectFunc);
    overloadData.emplace(objectFunc.getParams(), std::move(object));
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
    if (funcType.declType() != DeclType::Overload)
    {
        action->error("Makeing a function call with a non function object");
    }
    TypeCList   paramTypes;
    for (auto const& param: params)
    {
        paramTypes.emplace_back(param.get().getType());
    }
    Overload const& functionInfo = dynamic_cast<Overload const&>(funcType);
    return functionInfo.getReturnType(action, paramTypes);
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
