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

#if 0
class Decl
    class MemberInit: public Decl
    class Scope: public Decl
    class NamedScope: public Scope
        class Namespace: public NamedScope
        class Type: public NamedScope
            class Void: public Type
            class Class: public Type
            class Function: public Type
            class Overload: public Type
    class Object: public Decl
        class ObjectVariable: public Object
        class ObjectFunction: public Object
            class ObjectFunctionConstructor: public ObjectFunction
        class ObjectOverload: public Object
    class Statement: public Decl
        class StatementExpression: public Statement
        class StatementReturn: public Statement
        class StatementAssembley: public Statement
        class StatementCodeBlock: public Statement
    class Expression: public Decl
        class ExpressionObject: public Expression
        class ExpressionMemberAccess: public Expression
        class ExpressionLiteral: public Expression
        class ExpressionFuncCall: public Expression
#endif

class Action;
class Decl;
class Scope;
class CodeBlock;
class MemberInit;
class Namespace;
class Type;
class Void;
class Class;
class Function;
class Overload;
class Object;
class ObjectVariable;
class ObjectFunction;
class ObjectOverload;
class Statement;
class Expression;

enum class DeclType {Void, Namespace, Class, Function, Overload, CodeBlock, MemberInit, ObjectVariable, ObjectFunction, ObjectOverload, Statement, Expression};

using Int               = std::size_t;
using Identifier        = std::string;
using MemberStorage     = std::map<std::string, std::unique_ptr<Decl>>;
using MemberIndex       = std::map<std::string, std::size_t>;
using NameRef           = MemberStorage::const_iterator;

using ActionRef         = Action*;      // Pointer because Storage uses nullptr

using DeclRef           = std::reference_wrapper<Decl>;
using ScopeRef          = std::reference_wrapper<Scope>;
using CodeBlockRef      = std::reference_wrapper<CodeBlock>;
using MemberInitRef     = std::reference_wrapper<MemberInit>;
using NamespaceRef      = std::reference_wrapper<Namespace>;
using TypeRef           = std::reference_wrapper<Type>;
using TypeCRef          = std::reference_wrapper<Type const>;
using TypeCList         = std::list<TypeCRef>;
using VoidRef           = std::reference_wrapper<Void>;
using ClassRef          = std::reference_wrapper<Class>;
using FunctionRef       = std::reference_wrapper<Function>;
using FunctionCRef      = std::reference_wrapper<Function const>;
using OverloadRef       = std::reference_wrapper<Overload>;
using ObjectRef         = std::reference_wrapper<Object>;
using ObjectVariableRef = std::reference_wrapper<ObjectVariable>;
using ObjectVariableCRef= std::reference_wrapper<ObjectVariable const>;
using ObjectFunctionRef = std::reference_wrapper<ObjectFunction>;
using ObjectOverloadRef = std::reference_wrapper<ObjectOverload>;
using StatementRef      = std::reference_wrapper<Statement>;
using ExpressionRef     = std::reference_wrapper<Expression>;
using MemberInitList    = std::list<MemberInitRef>;
using StatementList     = std::list<StatementRef>;
using ExpressionList    = std::list<ExpressionRef>;

using NamespaceDecOrder = std::vector<NamespaceRef>;

inline bool operator<(TypeCRef const& lhs, TypeCRef const& rhs)
{
    return &lhs.get() < &rhs.get();
}
inline bool operator==(TypeCRef const& lhs, TypeCRef const& rhs)
{
    return &lhs.get() == &rhs.get();
}

class Decl
{
    public:
        Decl(ActionRef /*action*/)
        {
            // std::cerr << "Creating:   " << this << "\n";
        }
        virtual ~Decl()
        {
            // std::cerr << "Destroying: " << this << "\n";
        }
        Decl(Decl const&) = delete;
        Decl& operator=(Decl const&) = delete;
        Decl(Decl&&) = delete;
        Decl& operator=(Decl&&) = delete;
        virtual DeclType declType() const = 0;
        virtual std::string const& declName() const;
        virtual bool needsStorage() const;
        virtual bool overloadable() const;

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 9;
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
        Decl& saveMember(Action& action, std::unique_ptr<Decl>&& member);
        char hex(std::size_t halfByte);
        void generateHexName(std::string& name, std::size_t count);
};

class CodeBlock: public Scope
{
    public:
        using Scope::Scope;
        virtual std::string const& declName() const override {static std::string name;return name;}
        virtual DeclType declType() const override {return DeclType::CodeBlock;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 10;
};

class MemberInit: public Decl
{
    Identifier      name;
    ExpressionList  init;
    public:
        MemberInit(ActionRef action, Identifier name, ExpressionList init)
            : Decl(action)
            , name(std::move(name))
            , init(std::move(init))
        {}
        virtual std::string const& declName() const override {static std::string name;return name;}
        virtual DeclType declType() const override {return DeclType::MemberInit;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 11;
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
        static constexpr Int defaultStorageId = 12;
};

class Type: public NamedScope
{
    public:
        using NamedScope::NamedScope;
        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 13;
};

class Void: public Type
{
    public:
        Void(ActionRef action);
        virtual DeclType declType() const override {return DeclType::Void;}

        static constexpr bool valid = false;
        static constexpr Int defaultStorageId = 14;
};

class Class: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Class;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 15;
};

class Function: public Type
{
    TypeCList       param;
    Type const&     returnType;
    public:
        Function(ActionRef action, std::string name, TypeCList param, Type const& returnType)
            : Type(action, std::move(name))
            , param(std::move(param))
            , returnType(returnType)
        {}
        virtual DeclType declType() const override {return DeclType::Function;}
        virtual bool storeFunctionsInContainer() const override {return true;}
        Type const& getReturnType(ActionRef /*action*/) const {return returnType;}

        TypeCList getParams() const {return param;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 16;
};

class Overload: public Type
{
    std::map<TypeCList, FunctionCRef>   overloadData;
    public:
        Overload(ActionRef action, std::string name)
            : Type(action, std::move(name))
        {}
        virtual DeclType declType() const override {return DeclType::Overload;}
        Type const& getReturnType(ActionRef action, TypeCList const& index) const;
        void addOverload(Function const& type);

        // static constexpr bool valid = true;
        // static constexpr Int defaultStorageId = XX;
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
        virtual std::string const& declName() const override {return name;}
        virtual bool needsStorage() const override {return true;}
 
        Type const& getType() const {return type;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 17;
};

class ObjectVariable: public Object
{
    ExpressionList      init;
    public:
        ObjectVariable(ActionRef action, std::string name, Type const& type, ExpressionList init)
            : Object(action, std::move(name), type)
            , init(std::move(init))
        {}
        virtual DeclType declType() const override {return DeclType::ObjectVariable;}
};

class ObjectFunction: public Object
{
    Statement&  code;
    public:
        ObjectFunction(ActionRef action, std::string name, Type const& type, Statement& code)
            : Object(action, std::move(name), type)
            , code(code)
        {}
        virtual DeclType declType() const override {return DeclType::ObjectFunction;}
};

class ObjectFunctionConstructor: public ObjectFunction
{
    MemberInitList      init;
    public:
        ObjectFunctionConstructor(ActionRef action, std::string name, Type const& type, MemberInitList init, Statement& code)
            : ObjectFunction(action, std::move(name), type, code)
            , init(std::move(init))
        {}
};

class ObjectOverload: public Object
{
    std::map<TypeCList, std::unique_ptr<ObjectFunction>>      overloadData;
    public:
        ObjectOverload(ActionRef action, std::string name, Type const& type)
            : Object(action, std::move(name), type)
        {}
        virtual DeclType declType() const override {return DeclType::ObjectOverload;}
        virtual bool overloadable() const override {return true;}

        void addOverload(std::unique_ptr<Decl>&& decl);

        // static constexpr bool valid = true;
        // static constexpr Int defaultStorageId = XX;
};

class Statement: public Decl
{
    public:
        using Decl::Decl;
        virtual DeclType declType() const override {return DeclType::Statement;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 18;
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

class StatementAssembley: public Statement
{
    std::string     assembley;
    public:
        using Base = Statement;
        StatementAssembley(ActionRef action, std::string assembley)
            : Statement(action)
            , assembley(std::move(assembley))
        {}
};

class StatementCodeBlock: public Statement
{
    CodeBlock&      codeBlock;
    StatementList   list;
    public:
        using Base = Statement;
        StatementCodeBlock(ActionRef action, CodeBlock& codeBlock, StatementList list)
            : Statement(action)
            , codeBlock(codeBlock)
            , list(std::move(list))
        {}
};

class Expression: public Decl
{
    public:
        using Decl::Decl;
        virtual DeclType declType() const override {return DeclType::Expression;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 19;
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
        ExpressionMemberAccess(ActionRef action, Expression& src, Identifier const& memberName)
            : Expression(action)
            , src(src)
            , member(findMember(action, memberName))
        {}
        virtual Type const& getType() const override {return member.getType();}
    private:
        Object& findMember(ActionRef action, Identifier const& memberName);
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
        ExpressionLiteral(ActionRef action, T literal)
            : Expression(action)
            , literal(std::move(literal))
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
        ExpressionFuncCall(ActionRef action, Expression& funcObject, ExpressionList params)
            : Expression(action)
            , funcObject(funcObject)
            , params(std::move(params))
            , type(findType(action))
        {}
        virtual Type const& getType() const override {return type;}
    private:
        Type const& findType(ActionRef action);
};


template<typename T, typename... Args>
inline T& Scope::add(Action& action, Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(&action, std::forward<Args>(args)...));
    Decl& object = saveMember(action, std::move(decl));
    return dynamic_cast<T&>(object);
}


}

#endif
