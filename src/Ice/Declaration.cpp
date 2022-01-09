#include "Declaration.h"
#include "StorageAccess.h"
#include "Action.h"

#include <vector>
#include <set>

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

std::string const& Decl::declName(bool) const
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

void Scope::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope\n";
    }
    stream << indent(s+1) << "nextObjectId:  " << nextObjectId << "\n"
           << indent(s+1) << "anonNameCount: " << anonNameCount << "\n"
           << indent(s+1) << "members\n";
    for (auto const& member: members)
    {
        auto find = objectId.find(member.first);
        stream << indent(s+2) << member.first << "  size: " << find->second << "\n";
        member.second->print(stream, s+3, true);
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

Decl& Scope::saveMember(ActionRef action, std::unique_ptr<Decl>&& member)
{
    Decl& result = *member;

    std::string  index = member->declName();
    if (index == "")
    {
        index = anonName();
    }
    if (member->storageSize() != 0)
    {
        objectId[index] = nextObjectId;
        nextObjectId += member->storageSize();
    }


    auto& location = members[index];
    if (location.get() != nullptr)
    {
        action->error("Location already in use: ", index, "\n",
                      "\n"
                      "DeclarationName: ", location.get()->declName(), "\n"
                     );
    }
    else
    {
        location = std::move(member);
    }
    return result;
}

void CodeBlock::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::CodeBlock\n";
    }
    Scope::print(stream, s, false);
}

void MemberInit::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::MemberInit\n";
    }
    stream << indent(s+1) << "name: " << name << "\n"
           << indent(s+1) << "init\n";
    int count = 0;
    for (auto const& exp: init)
    {
        stream << indent(s+2) << "init " << count << "\n";
        exp.get().print(stream, s+3, true);
        ++count;
    }
}

void NamedScope::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope\n";
    }
    stream << indent(s+1) << "name: " << name << "\n"
           << indent(s+1) << "fullName: " << fullName << "\n";
    Scope::print(stream, s, false);
}

void Namespace::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Namespace\n";
    }
    NamedScope::print(stream, s, false);
}

void Type::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Type\n";
    }
    //NamedScope::print(stream, s, false);
}

Void& Void::getInstance()
{
    static Void instance;
    return instance;
}

Void::Void()
    : Type(nullptr, "Void")
{
}

void Void::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Type::Void\n";
    }
    //Type::print(stream, s, false);
}

void Class::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Type::Class\n";
    }
    Type::print(stream, s, false);
}

void Function::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Type::Function\n";
    }
    stream << indent(s+1) << "param\n";
    int count = 0;
    for (auto const& p: param)
    {
        stream << indent(s+2) << "param " << count << "\n";
        p.get().print(stream, s+3, true);
        ++count;
    }
    stream << indent(s+1) << "return type\n";
    returnType.print(stream, s+2, true);
    Type::print(stream, s, false);
}

std::string Function::getExtension() const
{
    std::stringstream ext;
    std::string sep = "";
    ext << "{";
    for (auto const& p: param)
    {
        ext << sep << p.get().declName();
        sep = ",";
    }
    ext << "}";
    return ext.str();
}

void Overload::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Scope::NamedScope::Type::Overload\n";
    }
    int count = 0;
    stream << indent(s+1) << "overloadData\n";
    for (auto const& sub: overloadData)
    {
        stream << indent(s+2) << "overloadData " << count << "\n";
        ++count;

        int k = 0;
        for (auto const& key: sub.first)
        {
            stream << indent(s+3) << "Key " << k << "\n";
            key.get().print(stream, s+4, true);
            ++k;
        }
        stream << indent(s+3) << "Value\n";
        sub.second.get().print(stream, s+4, true);
    }
    Type::print(stream, s, false);
}

Function const& Overload::getOverload(ActionRef action, TypeCList const& index) const
{
    auto find = overloadData.find(index);
    if (find == overloadData.end())
    {
        action->error("Did not find overlaad");
    }
    return find->second.get();
}

Type const& Overload::getReturnType(ActionRef action, TypeCList const& index) const
{
    Function const& func = getOverload(action, index);
    return func.getReturnType(action);
}

bool Overload::findMatch(ActionRef, TypeCList const& index) const
{
    auto find = overloadData.find(index);
    return find != overloadData.end();
}

void Overload::addOverload(Function const& type)
{
    overloadData.emplace(type.getParams(), FunctionCRef{type});
}

void Object::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Object\n";
    }
    stream << indent(s+1) << "name: " << name << "\n";
    stream << indent(s+1) << "type\n";
    type.print(stream, s+2, true);
}

void ObjectVariable::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Object::ObjectVariable\n";
    }
    Object::print(stream, s, false);
    int count = 0;
    stream << indent(s+1) << "init\n";
    for (auto const& exp: init)
    {
        stream << indent(s+2) << "init " << count << "\n";
        exp.get().print(stream, s+3, true);
        ++count;
    }
}

void ObjectFunction::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Object::ObjectFunction\n";
    }
    Object::print(stream, s, false);
    stream << indent(s+1) << "code\n";
    code.print(stream, s+2, true);
    int count = 0;
    stream << indent(s+1) << "init\n";
    for (auto const& i: init)
    {
        stream << indent(s+2) << "init " << count << "\n";
        i.get().print(stream, s+3, true);
        ++count;
    }
}

void ObjectFunction::addMissingMemberInit(ActionRef action, Scope& scope, MemberList const& members, bool con)
{
    MemberInitList::iterator    dstLoop = init.begin();
    MemberInitList::iterator    dstEnd  = init.end();
    MemberList::const_iterator  srcLoop = members.begin();
    MemberList::const_iterator  srcEnd  = members.end();

    while (dstLoop != dstEnd && srcLoop != srcEnd)
    {
        if (dstLoop->get().getName() == srcLoop->get().declName())
        {
            addMemberInit(action, scope, *srcLoop, std::move(dstLoop->get()), con);
            ++dstLoop;
            ++srcLoop;
            continue;
        }
        addMemberInit(action, scope, *srcLoop, MemberInit{action, srcLoop->get().declName(), ExpressionList{}}, con);
        ++srcLoop;
    }
    if (dstLoop != dstEnd)
    {
        action->error("Members are initialized in wrong order");
    }
    while (srcLoop != srcEnd)
    {
        addMemberInit(action, scope, *srcLoop, MemberInit{action, srcLoop->get().declName(), ExpressionList{}}, con);
        ++srcLoop;
    }
}

void ObjectFunction::addMemberInit(ActionRef action, Scope& scope, ObjectRef data, MemberInit&& memberInit, bool con)
{
    ObjectFunction* function = dynamic_cast<ObjectFunction*>(&data.get());
    if (function != nullptr)
    {
        std::cerr << "Ignoring: " << function->declName() << "\n";
        return;
    }
    Expression& object      = scope.add<ExpressionObject>(action, data);
    std::string methodName = con ? "$constructor" : "$destructor";
    ExpressionMemberAccess& method = scope.add<ExpressionMemberAccess>(action, object, std::move(methodName));
    Expression& callInit    = scope.add<ExpressionFuncCall>(action, method, std::move(memberInit.expressionList()));
    Statement&  initMember  = scope.add<StatementExpression>(action, callInit);
    code.prefix(initMember);
}


int ObjectOverload::storageSize() const
{
    return std::accumulate(std::begin(overloadData), std::end(overloadData), 0,
                            [](int lhs, auto const& rhs)
                            {
                                return lhs + rhs.second.get().storageSize();
                            });
}

void ObjectOverload::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Object::ObjectOverload\n";
    }
    Object::print(stream, s, false);
    // Note: This is passed to Object base class where a refernece is held.
    //       when the Object::print is called it will be printed there.
    // stream << indent(s+1) << "overloadType\n";
    // overloadType.print(stream, s+2, true);
    stream << indent(s+1) << "overloadData\n";
    int count = 0;
    for (auto const& data: overloadData)
    {
        stream << indent(s+2) << "overloadData " << count << "\n";
        ++count;

        int k = 0;
        for (auto const& key: data.first)
        {
            stream << indent(s+3) << "Key " << k << "\n";
            key.get().print(stream, s+4, true);
            ++k;
        }
        stream << indent(s+3) << "Value\n";
        data.second.get().print(stream, s+4, true);
    }
}

void ObjectOverload::addOverload(ObjectFunction& func)
{
    Type&       type = const_cast<Type&>(getType());
    Overload&   overload = dynamic_cast<Overload&>(type);

    Type&       objectType = const_cast<Type&>(func.getType());
    Function&   objectFunc = dynamic_cast<Function&>(objectType);

    overload.addOverload(objectFunc);
    overloadData.emplace(objectFunc.getParams(), ObjectFunctionRef{func});
}

void Statement::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Statement\n";
    }
}

void StatementExpression::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Statement::StatementExpression\n";
    }
    Statement::print(stream, s, false);
    stream << indent(s+1) << "expression\n";
    expression.print(stream, s+2, true);
}

void StatementReturn::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Statement::StatementReturn\n";
    }
    Statement::print(stream, s, false);
    stream << indent(s+1) << "expression\n";
    expression.print(stream, s+2, true);
}

void StatementAssembley::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Statement::StatementAssembley\n";
    }
    Statement::print(stream, s, false);
    stream << indent(s+1) << "assembley\n";
    std::stringstream assembleyStream(assembley);
    std::string line;
    while (std::getline(assembleyStream, line))
    {
        stream << indent(s+2) << line << "\n";
    }
}

void StatementCodeBlock::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Statement::StatementCodeBlock\n";
    }
    Statement::print(stream, s, false);
    stream << indent(s+1) << "codeBlock\n";
    codeBlock.print(stream, s+2, true);
    int count = 0;
    stream << indent(s+1) << "list\n";
    for (auto const& stat: list)
    {
        stream << indent(s+2) << "list " << count << "\n";
        stat.get().print(stream, s+3, true);
        ++count;
    }
}

void Expression::print(std::ostream& /*stream*/, int /*s*/, bool showName) const
{
    // Expression is a virtual class so can not be printed.
    if (showName == true)
    {
        throw std::runtime_error("Should never be called with true");
    }
    // The next class down is Decl (there is no implementation of print so no further action).
}

void ExpressionObject::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Expression::ExpressionObject\n";
    }
    Expression::print(stream, s, false);
    stream << indent(s+1) << "object\n";
    object.print(stream, s+2, true);
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

void ExpressionMemberAccess::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Expression::ExpressionMemberAccess\n";
    }
    Expression::print(stream, s, false);
    stream << indent(s+1) << "src\n";
    src.print(stream, s+2, true);
    stream << indent(s+1) << "member\n";
    member.print(stream, s+2, true);
}

Type const& ExpressionFuncCall::findType(ActionRef action)
{
    TypeCList   paramTypes;
    for (auto const& param: params)
    {
        paramTypes.emplace_back(param.get().getType());
    }

    Type const& funcType = funcObject.getType();
    Overload const* functionInfo = dynamic_cast<Overload const*>(&funcType);
    if (functionInfo == nullptr)
    {
        action->error("Makeing a function call with a non function object");
    }
    return functionInfo->getReturnType(action, paramTypes);
}

void ExpressionFuncCall::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Expression::ExpressionFuncCall\n";
    }
    Expression::print(stream, s, false);
    stream << indent(s+1) << "funcObject\n";
    funcObject.print(stream, s+2, true);
    stream << indent(s+1) << "type\n";
    type.print(stream, s+2, true);
    int count = 0;
    stream << indent(s+1) << "params\n";
    for (auto const& p: params)
    {
        stream << indent(s+2) << "params " << count << "\n";
        ++count;

        p.get().print(stream, s+3, true);
    }
}


namespace ThorsAnvil::Anvil::Ice
{
    template class ExpressionLiteral<std::string>;
    template class ExpressionLiteral<Int>;
}
