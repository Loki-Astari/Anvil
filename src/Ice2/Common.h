#ifndef THORSANVIL_ANVIL_ICE_COMMON_H
#define THORSANVIL_ANVIL_ICE_COMMON_H

#include <map>
#include <list>

#if 0
class Decl
    class MemberInit: public Decl
    class Scope: public Decl
        class CodeBlock: public Scope
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
namespace ThorsAnvil::Anvil::Ice
{

// Action.h
class Action;

// Declaration.h
class Decl;
class MemberInit;
class Scope;
class CodeBlock;
class Namespace;
class Type;
class Void;
class Class;
class Function;
class Object;
class ObjectVariable;
class Statement;
class Expression;

template<typename T, bool valid = true> struct Id;
template<typename T>                    struct IdAccess;

template<typename T> using Ref              = std::reference_wrapper<T>;
template<typename T> using List             = std::list<Ref<T>>;
template<typename T> using ListId           = Id<List<T>>;
template<typename T> using ListAccess       = IdAccess<List<T>>;
template<typename T> using Base             = typename T::Base;


using Int                   = std::size_t;

using Identifier            = std::string;
using IdentifierId          = Id<Identifier>;

using ActionRef             = Action*;      // Pointer because Storage uses nullptr

using MemberStorage         = std::map<std::string, std::unique_ptr<Decl>>;
using MemberIndex           = std::map<std::string, std::size_t>;
using NameRef               = MemberStorage::const_iterator;

using DeclRef               = std::reference_wrapper<Decl>;
using DeclList              = std::list<DeclRef>;
using DeclListId            = Id<DeclList>;

using MemberInitId          = Id<MemberInit>;
using MemberInitRef         = std::reference_wrapper<MemberInit>;
using MemberInitList        = std::list<MemberInitRef>;
using MemberInitListId      = Id<MemberInitList>;

using ScopeId               = Id<Scope>;
using ScopeRef              = std::reference_wrapper<Scope>;

using CodeBlockId           = Id<CodeBlock>;

using NamespaceId           = Id<Namespace>;
using NamespaceRef          = std::reference_wrapper<Namespace>;
using NamespaceList         = std::list<NamespaceRef>;
using NamespaceListId       = Id<NamespaceList>;

using TypeId                = Id<Type>;
using TypeCRef              = std::reference_wrapper<Type const>;
using TypeCList             = std::list<TypeCRef>;
using TypeCListId           = Id<TypeCList>;

using VoidId                = Id<Void, false>;
using ClassId               = Id<Class>;

using FunctionId            = Id<Function>;
using FunctionCRef          = std::reference_wrapper<Function const>;

using ObjectId              = Id<Object>;
using ObjectVariableCRef    = std::reference_wrapper<ObjectVariable const>;

using StatementId           = Id<Statement>;
using StatementRef          = std::reference_wrapper<Statement>;
using StatementList         = std::list<StatementRef>;
using StatementListId       = Id<StatementList>;

using ExpressionId          = Id<Expression>;
using ExpressionRef         = std::reference_wrapper<Expression>;
using ExpressionList        = std::list<ExpressionRef>;
using ExpressionListId      = Id<ExpressionList>;

inline bool operator<(TypeCRef const& lhs, TypeCRef const& rhs)
{
    return &lhs.get() < &rhs.get();
}
inline bool operator==(TypeCRef const& lhs, TypeCRef const& rhs)
{
    return &lhs.get() == &rhs.get();
}

}

#endif
