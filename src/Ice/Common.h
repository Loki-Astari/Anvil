#ifndef THORSANVIL_ANVIL_ICE_COMMON_H
#define THORSANVIL_ANVIL_ICE_COMMON_H

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <functional>

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
            class ObjectFunctionSpecial: public ObjectFunction
                class ObjectFunctionConstructor: public ObjectFunctionSpecial
                class ObjectFunctionDestructor: public ObjectFunctionSpecial
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
class ObjectFunction;
class Statement;
class StatementCodeBlock;
class Expression;

template<typename T, bool valid = true> struct Id;
template<typename T>                    struct IdAccess;

template<typename T> using Ref              = std::reference_wrapper<T>;
template<typename T> using List             = std::list<Ref<T>>;
template<typename T> using ListId           = Id<List<T>>;
template<typename T> using ListAccess       = IdAccess<List<T>>;
template<typename T> using Base             = typename T::Base;


using Int                   = std::size_t;
using IntId                 = Id<Int>;

using Identifier            = std::string;
using IdentifierId          = Id<Identifier>;
using IdentifierAccess      = IdAccess<Identifier>;

using ActionRef             = Action*;      // Pointer because Storage uses nullptr

using MemberStorage         = std::map<std::string, std::unique_ptr<Decl>>;
using MemberIndex           = std::map<std::string, std::size_t>;
using NameRef               = MemberStorage::const_iterator;
using ScopeIter             = MemberStorage::const_iterator;

using DeclId                = Id<Decl>;
using DeclRef               = std::reference_wrapper<Decl>;
using DeclList              = std::list<DeclRef>;
using DeclListId            = Id<DeclList>;
using DeclListAccess        = IdAccess<DeclList>;

using MemberInitId          = Id<MemberInit>;
using MemberInitRef         = std::reference_wrapper<MemberInit>;
using MemberInitList        = std::list<MemberInitRef>;
using MemberInitListId      = Id<MemberInitList>;
using MemberInitListAccess  = IdAccess<MemberInitList>;

using ScopeId               = Id<Scope>;
using ScopeAccess           = IdAccess<Scope>;
using ScopeRef              = std::reference_wrapper<Scope>;

using CodeBlockId           = Id<CodeBlock>;
using CodeBlockAccess       = IdAccess<CodeBlock>;
using CodeBlockRef          = std::reference_wrapper<CodeBlock>;

using NamespaceId           = Id<Namespace>;
using NamespaceAccess       = IdAccess<Namespace>;
using NamespaceRef          = std::reference_wrapper<Namespace>;
using NamespaceList         = std::list<NamespaceRef>;
using NamespaceListId       = Id<NamespaceList>;
using NamespaceListAccess   = IdAccess<NamespaceList>;

using TypeId                = Id<Type>;
using TypeAccess            = IdAccess<Type>;
using TypeRef               = std::reference_wrapper<Type>;
using TypeCRef              = std::reference_wrapper<Type const>;
using TypeCList             = std::list<TypeCRef>;
using TypeCListId           = Id<TypeCList>;
using TypeCListAccess       = IdAccess<TypeCList>;

using VoidId                = Id<Void, false>;
using VoidRef               = std::reference_wrapper<Void>;

using ClassId               = Id<Class>;
using ClassAccess           = IdAccess<Class>;
using ClassRef              = std::reference_wrapper<Class>;

using FunctionId            = Id<Function>;
using FunctionAccess        = IdAccess<Function>;
using FunctionRef           = std::reference_wrapper<Function>;
using FunctionCRef          = std::reference_wrapper<Function const>;

using ObjectId              = Id<Object>;
using ObjectRef             = std::reference_wrapper<Object>;
using ObjectCRef            = std::reference_wrapper<Object const>;
using ObjectFunctionRef     = std::reference_wrapper<ObjectFunction>;
using ObjectAccess          = IdAccess<Object>;

using StatementId           = Id<Statement>;
using StatementAccess       = IdAccess<Statement>;
using StatementRef          = std::reference_wrapper<Statement>;
using StatementList         = std::list<StatementRef>;
using StatementListId       = Id<StatementList>;
using StatementListAccess   = IdAccess<StatementList>;

using StatementCodeBlockId      = Id<StatementCodeBlock>;
using StatementCodeBlockAccess  = IdAccess<StatementCodeBlock>;
using StatementCodeBlockRef     = std::reference_wrapper<StatementCodeBlock>;

using ExpressionId          = Id<Expression>;
using ExpressionRef         = std::reference_wrapper<Expression>;
using ExpressionList        = std::list<ExpressionRef>;
using ExpressionListId      = Id<ExpressionList>;
using ExpressionListAccess  = IdAccess<ExpressionList>;

using MemberList            = std::vector<ObjectRef>;
using OverloadMap           = std::map<TypeCList, ObjectFunctionRef>;
using OverloadIter          = OverloadMap::iterator;
using NamespaceDecOrder     = std::vector<NamespaceRef>;

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
