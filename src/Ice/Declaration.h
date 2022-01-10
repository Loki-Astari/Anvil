#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include "Common.h"
#include <map>
#include <string>
#include <ostream>
#include <memory>
#include <utility>

namespace ThorsAnvil::Anvil::Ice
{

enum class DeclGroup { Declaration, Data, Function};
class Generator;
class Decl
{
    public:
        Decl(ActionRef /*action*/);
        virtual ~Decl();

        Decl(Decl const&) = delete;
        Decl& operator=(Decl const&) = delete;
        Decl(Decl&&) = delete;
        Decl& operator=(Decl&&) = delete;

        virtual std::string const& declName(bool = false) const;
        virtual DeclGroup declGroup() const {return DeclGroup::Declaration;}
        virtual int  storageSize() const    {return 0;}
        virtual bool overloadable() const   {return false;}
        virtual void print(std::ostream&, int indent, bool showName) const = 0;
        virtual void generateCode(Generator& gen, std::ostream& output) const;

        static constexpr bool valid = true;
    protected:
        std::string indent(int size) const {return std::string(size * 4, ' ');}
};

inline std::ostream& operator<<(std::ostream& stream, Decl const& decl)
{
    decl.print(stream, 0, true);
    return stream;
}

/*
class ScopeIterator
{
    ScopeIter    iter;
    public:
        ScopeIterator(){}
        ScopeIterator(ScopeIterator const& copy): iter(copy.iter) {}
        ScopeIterator operator=(ScopeIterator const& copy) {iter = copy.iter;return *this;}
        ScopeIterator(ScopeIter iter): iter(iter) {}
        ScopeIterator operator++(){++iter;return *this;}
        ScopeIterator operator++(int){ScopeIterator tmp(iter);++iter;return tmp;}
        Decl& operator*() {return *iter->second;}
        bool operator==(ScopeIterator const& rhs) const {return iter == rhs.iter;}
        bool operator!=(ScopeIterator const& rhs) const {return iter != rhs.iter;}
};
*/

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
        T& add(ActionRef action, Args&&... args);

        virtual bool storeFunctionsInContainer() const {return false;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        std::string anonName();

        std::size_t getMemberSize() const {return nextObjectId;}

    private:
        Decl& saveMember(ActionRef action, std::unique_ptr<Decl>&& member);
        char hex(std::size_t halfByte);
        void generateHexName(std::string& name, std::size_t count);
};

class CodeBlock: public Scope
{
    public:
        using Scope::Scope;
        virtual std::string const& declName(bool = false) const override {static std::string name;return name;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;

        static constexpr bool valid = true;
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
        virtual std::string const& declName(bool = false) const override {static std::string name;return name;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        Identifier const& getName() {return name;}

        ExpressionList const& expressionList() const {return init;}

        static constexpr bool valid = true;
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
        virtual std::string const& declName(bool full = false) const override {return full && fullName != "" ? fullName : name;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        void setPath(std::string path) {fullName = std::move(path);}
};

class FunctionIterator
{
    DeclCIter    iter;
    public:
        FunctionIterator(){}
        FunctionIterator(FunctionIterator const& copy): iter(copy.iter) {}
        FunctionIterator operator=(FunctionIterator const& copy) {iter = copy.iter;return *this;}
        FunctionIterator(DeclCIter iter): iter(iter) {}
        FunctionIterator operator++(){++iter;return *this;}
        FunctionIterator operator++(int){FunctionIterator tmp(iter);++iter;return tmp;}
        Decl& operator*() {return *iter;}
        bool operator==(FunctionIterator const& rhs) const {return iter == rhs.iter;}
        bool operator!=(FunctionIterator const& rhs) const {return iter != rhs.iter;}
};

class Namespace: public NamedScope
{
    DeclList    functions;
    DeclList    objects;
    DeclList    other;
    public:
        Namespace(ActionRef action, std::string name)
            : NamedScope(action, std::move(name))
        {}

        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        virtual void generateCode(Generator& gen, std::ostream& output) const override;

        void addDecl(DeclList declList)
        {
            for (auto& decl: declList)
            {
                switch (decl.get().declGroup())
                {
                    case DeclGroup::Declaration:    other.emplace_back(decl);       break;
                    case DeclGroup::Data:           objects.emplace_back(decl);     break;
                    case DeclGroup::Function:       functions.emplace_back(decl);   break;
                    default:
                        throw std::domain_error("Unknown DeclGroup");
                }
            }
        }
        FunctionIterator    begin() const {return FunctionIterator(functions.begin());}
        FunctionIterator    end()   const {return FunctionIterator(functions.end());}
        static constexpr bool valid = true;
};

class Type: public NamedScope
{
    public:
        using NamedScope::NamedScope;
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        virtual std::string getExtension() const {return declName();}

        static constexpr bool valid = true;
};

class Class: public Type
{
    DeclList    functions;
    DeclList    objects;
    DeclList    other;
    public:
        using Type::Type;
        virtual bool storeFunctionsInContainer() const override {return true;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;

        void addDecl(DeclList declList)
        {
            for (auto& decl: declList)
            {
                switch (decl.get().declGroup())
                {
                    case DeclGroup::Declaration:    other.emplace_back(decl);       break;
                    case DeclGroup::Data:           objects.emplace_back(decl);     break;
                    case DeclGroup::Function:       functions.emplace_back(decl);   break;
                    default:
                        throw std::domain_error("Unknown DeclGroup");
                }
            }
        }
        FunctionIterator    begin() const {return FunctionIterator(functions.begin());}
        FunctionIterator    end()   const {return FunctionIterator(functions.end());}

        static constexpr bool valid = true;
};

class Void: public Type
{
    private:
        Void();
    public:
        static Void& getInstance();
        virtual void print(std::ostream& stream, int indent, bool showName) const override;

        static constexpr bool valid = false;
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
        virtual bool storeFunctionsInContainer() const override {return true;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        virtual std::string getExtension() const override;
        Type const& getReturnType(ActionRef /*action*/) const {return returnType;}
        TypeCList getParams() const {return param;}

        static constexpr bool valid = true;
};

class Overload: public Type
{
    std::map<TypeCList, FunctionCRef>   overloadData;
    public:
        Overload(ActionRef action)
            : Type(action, "")
        {}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        Function const& getOverload(ActionRef action, TypeCList const& index) const;
        Type const& getReturnType(ActionRef action, TypeCList const& index) const;
        bool findMatch(ActionRef, TypeCList const& index) const;
        void addOverload(Function const& type);
};

class Object: public NamedScope
{
    Type const&     type;
    public:
        Object(ActionRef action, std::string name, Type const& type)
            : NamedScope(action, std::move(name))
            , type(type)
        {}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        Type const& getType() const {return type;}

        static constexpr bool valid = true;
};

class ObjectVariable: public Object
{
    ExpressionList      init;
    public:
        ObjectVariable(ActionRef action, std::string name, Type const& type, ExpressionList init)
            : Object(action, std::move(name), type)
            , init(std::move(init))
        {}
        virtual DeclGroup declGroup() const override {return DeclGroup::Data;}
        virtual int storageSize() const override {return 1;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
};

class ObjectFunction: public Object
{
    friend class Generator;
    protected:
        StatementCodeBlock*  code;
        MemberInitList       init;
    public:
        ObjectFunction(ActionRef action, std::string name, Type const& type)
            : Object(action, name += type.getExtension(), type)
            , code(nullptr)
        {}
        virtual DeclGroup declGroup() const override {return DeclGroup::Function;}
        virtual int storageSize() const override {return 0;}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        virtual void generateCode(Generator& gen, std::ostream& output) const override;

        void addCode(StatementCodeBlock& codeRef, MemberInitList initList)
        {
            code    = &codeRef;
            init    = std::move(initList);
        }
        void addMissingMemberInit(ActionRef action, Scope& scope, MemberList const& members, bool con);
        void addMemberInit(ActionRef action, Scope& scope, ObjectRef data, MemberInit&& memberInit, bool con);
};


class OverloadIterator
{
    OverloadIter    iter;
    public:
        OverloadIterator(){}
        OverloadIterator(OverloadIterator const& copy): iter(copy.iter) {}
        OverloadIterator operator=(OverloadIterator const& copy) {iter = copy.iter;return *this;}
        OverloadIterator(OverloadIter iter): iter(iter) {}
        OverloadIterator operator++(){++iter;return *this;}
        OverloadIterator operator++(int){OverloadIterator tmp(iter);++iter;return tmp;}
        ObjectFunction& operator*() {return iter->second.get();}
        bool operator==(OverloadIterator const& rhs) const {return iter == rhs.iter;}
        bool operator!=(OverloadIterator const& rhs) const {return iter != rhs.iter;}
};

class ObjectOverload: public Object
{
    Overload    overloadType;
    OverloadMap overloadData;
    public:
        ObjectOverload(ActionRef action, std::string name)
            : Object(action, std::move(name), overloadType)
            , overloadType(action)
        {}
        virtual bool overloadable() const override {return true;}
        virtual int storageSize() const override;
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        void addOverload(ObjectFunction& func);

        OverloadIterator begin()    {return OverloadIterator(overloadData.begin());}
        OverloadIterator end()      {return OverloadIterator(overloadData.end());}
};

class Statement: public Decl
{
    public:
        using Decl::Decl;
        virtual void print(std::ostream& stream, int indent, bool showName) const override;

        static constexpr bool valid = true;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
};

class StatementIterator
{
    StatementCIter    iter;
    public:
        StatementIterator(){}
        StatementIterator(StatementIterator const& copy): iter(copy.iter) {}
        StatementIterator operator=(StatementIterator const& copy) {iter = copy.iter;return *this;}
        StatementIterator(StatementCIter iter): iter(iter) {}
        StatementIterator operator++(){++iter;return *this;}
        StatementIterator operator++(int){StatementIterator tmp(iter);++iter;return tmp;}
        Decl& operator*() {return *iter;}
        bool operator==(StatementIterator const& rhs) const {return iter == rhs.iter;}
        bool operator!=(StatementIterator const& rhs) const {return iter != rhs.iter;}
};

class StatementCodeBlock: public Statement
{
    friend class Generator;
    CodeBlock&      codeBlock;
    StatementList   list;
    public:
        using Base = Statement;
        StatementCodeBlock(ActionRef action, CodeBlock& codeBlock, StatementList list)
            : Statement(action)
            , codeBlock(codeBlock)
            , list(std::move(list))
        {}
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
        virtual void generateCode(Generator& gen, std::ostream& output) const override;

        void prefix(Statement& pre)     {list.emplace_front(pre);}
        void postfix(Statement& post)   {list.emplace_back(post);}

        StatementIterator begin() const {return StatementIterator(list.begin());}
        StatementIterator end() const   {return StatementIterator(list.end());}
};

class Expression: public Decl
{
    public:
        using Decl::Decl;
        virtual Type const& getType() const = 0;
        virtual void print(std::ostream& stream, int indent, bool showName) const override;

        static constexpr bool valid = true;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
    private:
        Object& findMember(ActionRef action, Identifier const& memberName);
};

template<typename T> struct ExpressionLiteralTypeName;
template<> struct ExpressionLiteralTypeName<std::string> {static const std::string standardName;};
template<> struct ExpressionLiteralTypeName<Int>         {static const std::string standardName;};

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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
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
        virtual void print(std::ostream& stream, int indent, bool showName) const override;
    private:
        Type const& findType(ActionRef action);
};

}

#include "Declaration.tpp"

#endif
