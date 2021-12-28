#ifndef THORSANVIL_ANVIL_ICE_COMMON_H
#define THORSANVIL_ANVIL_ICE_COMMON_H

#include <list>

namespace ThorsAnvil::Anvil::Ice
{

class Scope;
class Function;
class Void;
class Namespace;
class Class;
class Decl;
class Type;
class CodeBlock;
class Statement;
class Object;
class Expression;
class MemberInit;

template<typename T, bool valid = true>
struct Id;
template<typename T>
struct IdAccess;

using ScopeRef = std::reference_wrapper<Scope>;
using VoidId                = Id<Void, false>;
using NamespaceId           = Id<Namespace>;
using ClassId               = Id<Class>;
using FunctionId            = Id<Function>;

using NamespaceRef          = std::reference_wrapper<Namespace>;
using NamespaceList         = std::list<NamespaceRef>;
using NamespaceListId       = Id<NamespaceList>;

using Identifier            = std::string;
using IdentifierId          = Id<Identifier>;

using DeclRef               = std::reference_wrapper<Decl>;
using DeclList              = std::list<DeclRef>;
using DeclListId            = Id<DeclList>;

using TypeCRef              = std::reference_wrapper<Type const>;
using TypeCList             = std::list<TypeCRef>;
using TypeCListId           = Id<TypeCList>;

using StatementRef          = std::reference_wrapper<Statement>;
using StatementList         = std::list<StatementRef>;
using StatementListId       = Id<StatementList>;

using ExpressionRef         = std::reference_wrapper<Expression>;
using ExpressionList        = std::list<ExpressionRef>;
using ExpressionListId      = Id<ExpressionList>;

using MemberInitRef         = std::reference_wrapper<MemberInit>;
using MemberInitList        = std::list<MemberInitRef>;
using MemberInitListId      = Id<MemberInitList>;

using TypeId                = Id<Type>;
using CodeBlockId           = Id<CodeBlock>;
using StatementId           = Id<Statement>;
using ObjectId              = Id<Object>;
using MemberInitId          = Id<MemberInit>;
using ExpressionId          = Id<Expression>;
using ScopeId               = Id<Scope>;

using Int                   = std::size_t;

template<typename T> using Ref              = std::reference_wrapper<T>;
template<typename T> using List             = std::list<Ref<T>>;
template<typename T> using ListId           = Id<List<T>>;
template<typename T> using ListAccess       = IdAccess<List<T>>;
template<typename T> using Base             = typename T::Base;

}

#endif
