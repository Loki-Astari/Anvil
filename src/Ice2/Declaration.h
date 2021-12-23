#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include <functional>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <iostream>

namespace ThorsAnvil::Anvil::Ice
{

class Action;
class Decl;
class Scope;
class Namespace;
class Type;
class Void;
class Class;
class Function;
class Object;
class ObjectInit;
class Statement;
class Expression;

enum class DeclType {Void, Namespace, Class, Function, Object, ObjectInit, Statement, Expression};

using Int               = std::size_t;
using Identifier        = std::string;
using MemberStorage     = std::map<std::string, std::unique_ptr<Decl>>;
using MemberIndex       = std::map<std::string, std::size_t>;
using NameRef           = MemberStorage::const_iterator;

using ActionRef         = Action*;      // Pointer because Storage uses nullptr

using DeclRef           = std::reference_wrapper<Decl>;
using ScopeRef          = std::reference_wrapper<Scope>;
using NamespaceRef      = std::reference_wrapper<Namespace>;
using TypeRef           = std::reference_wrapper<Type>;
using TypeList          = std::list<TypeRef>;
using VoidRef           = std::reference_wrapper<Void>;
using ClassRef          = std::reference_wrapper<Class>;
using FunctionRef       = std::reference_wrapper<Function>;
using ObjectRef         = std::reference_wrapper<Object>;
using ObjectInitRef     = std::reference_wrapper<ObjectInit>;
using StatementRef      = std::reference_wrapper<Statement>;
using ExpressionRef     = std::reference_wrapper<Expression>;
using ExpressionList    = std::list<ExpressionRef>;

using NamespaceDecOrder = std::vector<NamespaceRef>;

inline bool operator<(TypeRef const& lhs, TypeRef const& rhs)
{
    return &lhs.get() < &rhs.get();
}
inline bool operator==(TypeRef const& lhs, TypeRef const& rhs)
{
    return &lhs.get() == &rhs.get();
}

class Decl
{
    public:
        Decl(ActionRef /*action*/) {}
        virtual ~Decl() {}
        virtual DeclType declType() const = 0;
        virtual std::string const& declName() const;

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 8;
};


class Scope: public Decl
{
    MemberStorage   members;
    MemberIndex     objectId;
    std::size_t     nextObjectId;
    std::size_t     anonNameCount;
    public:
        Scope(ActionRef);

        std::pair<bool, NameRef> get(std::string const& name) const;
        template<typename T, typename... Args>
        T& add(Action& action, Args&&... args);
        virtual bool storeFunctionsInContainer() const {return false;}

        friend std::ostream& operator<<(std::ostream& str, Scope& data);

        std::string anonName();
    private:
        char hex(std::size_t halfByte);
        void generateHexName(std::string& name, std::size_t count);
};

class NamedScope: public Scope
{
    std::string name;
    std::string fullName;
    public:
        NamedScope(ActionRef action, std::string name)
            : Scope(action)
            , name(std::move(name))
        {}
        virtual std::string const& declName() const override {return name;}
        void setPath(std::string path) {fullName = std::move(path);}
};

class Namespace: public NamedScope
{
    public:
        Namespace(ActionRef action, std::string name)
            : NamedScope(action, std::move(name))
        {}
        virtual DeclType declType() const override {return DeclType::Namespace;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 9;
};

class Type: public NamedScope
{
    public:
        using NamedScope::NamedScope;
        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 10;
};

class Void: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Void;}

        static constexpr bool valid = false;
        static constexpr Int defaultStorageId = 11;
};

class Class: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Class;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 12;
};

class Function: public Type
{
    std::map<TypeList, TypeRef>   overload;
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Function;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 13;

        void addOverload(ActionRef action, TypeList&& list, Type& returnType);
        Type const& getReturnType(ActionRef action, ExpressionList const& params) const;
};

class Object: public Decl
{
    std::string     name;
    Type const&     type;
    public:
        Object(ActionRef action, std::string name, Type const& type)
            : Decl(action)
            , name(std::move(name))
            , type(type)
        {}
        virtual DeclType declType() const override {return DeclType::Object;}
        virtual std::string const& declName() const override {return name;}

        Type const& getType() const {return type;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 14;
};

class ObjectInit: public Decl
{
    public:
        using Decl::Decl;
        virtual DeclType declType() const override {return DeclType::ObjectInit;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 15;
};

class Statement: public Decl
{
    public:
        using Decl::Decl;
        virtual DeclType declType() const override {return DeclType::Statement;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 16;
};

class StatementExpression: public Statement
{
    Expression&     expression;
    public:
        using Base = Statement;
        StatementExpression(ActionRef action, Expression& expression)
            : Statement(action)
            , expression(expression)
        {}
};

class StatementReturn: public Statement
{
    Expression&     expression;
    public:
        using Base = Statement;
        StatementReturn(ActionRef action, Expression& expression)
            : Statement(action)
            , expression(expression)
        {}
};

class Expression: public Decl
{
    public:
        using Decl::Decl;
        virtual DeclType declType() const override {return DeclType::Expression;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 17;
        virtual Type const& getType() const = 0;
};

class ExpressionObject: public Expression
{
    Object&     object;
    public:
        using Base = Expression;
        ExpressionObject(ActionRef action, Object& object)
            : Expression(action)
            , object(object)
        {}
        virtual Type const& getType() const override {return object.getType();}
};

class ExpressionMemberAccess: public Expression
{
    Expression& src;
    Object&     member;
    public:
        using Base = Expression;
        ExpressionMemberAccess(ActionRef action, Expression& src, Identifier& memberName)
            : Expression(action)
            , src(src)
            , member(findMember(action, src, memberName))
        {}
        virtual Type const& getType() const override {return member.getType();}
    private:
        Object& findMember(ActionRef action, Expression& src, Identifier& memberName);
};

template<typename T>
struct ExpressionLiteralTypeName;

template<>
struct ExpressionLiteralTypeName<std::string>
{
    static const std::string standardName;
};
template<>
struct ExpressionLiteralTypeName<Int>
{
    static const std::string standardName;
};


template<typename T>
class ExpressionLiteral: public Expression
{
    T literal;
    Type const& type;
    public:
        using Base = Expression;
        ExpressionLiteral(ActionRef action, T& literal)
            : Expression(action)
            , literal(literal)
            , type(findType(action))
        {}
        virtual Type const& getType() const override {return type;}
    private:
        Type const& findType(ActionRef action);
};

class ExpressionFuncCall: public Expression
{
    Expression&     funcObject;
    ExpressionList  params;
    Type const&     type;
    public:
        using Base = Expression;
        ExpressionFuncCall(ActionRef action, Expression& funcObject, ExpressionList& params)
            : Expression(action)
            , funcObject(funcObject)
            , params(params)
            , type(findType(action))
        {}
        virtual Type const& getType() const override {return type;}
    private:
        Type const& findType(ActionRef action);
};


// Included from Declaration.h

// By default only object need runtime storage.
// So most types simply return false.
template<typename T>
inline
bool doesDeclNeedRuntimeStorage(Scope&, Decl const&)
{
    return false;
}

/*
template<>
inline
bool doesDeclNeedRuntimeStorage<Object>(Scope& scope, Decl const& decl)
{
    Object const& object = dynamic_cast<Object const&>(decl);
    // If we don't store functions and thus is a function then return false.
    // Otherwise return true as:
    //      A: This is not a function so needs storage in the local context.
    //      B: Is a function and the local context says it needs to store it.
    return (!scope.storeFunctionsInContainer() && object.getType().declType() == DeclType::Func)
                ? false
                : true;
}
*/

template<typename T, typename... Args>
inline T& Scope::add(Action& action, Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(&action, args...));
    std::string  index = decl->declName();
    if (index == "")
    {
        index = anonName();
    }
    auto& location = members[index];
    if (doesDeclNeedRuntimeStorage<T>(*this, *decl))
    {
        objectId[index] = nextObjectId++;
    }
    location = std::move(decl);
    return dynamic_cast<T&>(*location);
}


}

#endif
