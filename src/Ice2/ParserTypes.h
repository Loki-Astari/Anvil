#ifndef THORSANVIL_ANVIL_ICE_PARSER_TYPES_H
#define THORSANVIL_ANVIL_ICE_PARSER_TYPES_H

#include "Declaration.h"

#include <functional>
#include <vector>
#include <list>

namespace ThorsAnvil::Anvil::Ice
{

using Int                   = std::size_t;
using NamespaceList         = std::list<NamespaceRef>;
using DeclList              = std::list<DeclRef>;
using TypeList              = std::list<TypeRef>;
using StatementList         = std::list<StatementRef>;
using ExpressionList        = std::list<ExpressionRef>;

template<typename T>
struct IdTraits
{
    static constexpr bool valid = T::valid;
    static constexpr Int defaultStorageId = T::defaultStorageId;
    using AccessType = std::reference_wrapper<T>;
};

template<>
struct IdTraits<Int>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 1;
    using AccessType = Int;
};
template<>
struct IdTraits<Identifier>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 2;
    using AccessType = std::string;
};
template<>
struct IdTraits<DeclList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 3;
    using AccessType = DeclList;
};
template<>
struct IdTraits<NamespaceList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 4;
    using AccessType = NamespaceList;
};
template<>
struct IdTraits<MemberInitList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 5;
    using AccessType = MemberInitList;
};
template<>
struct IdTraits<TypeList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 6;
    using AccessType = TypeList;
};
template<>
struct IdTraits<StatementList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 7;
    using AccessType = StatementList;
};
template<>
struct IdTraits<ExpressionList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 8;
    using AccessType = ExpressionList;
};


template<typename T, bool valid = true>
struct Id
{
    Int value;
    Id(Int v)
        : value(v)
    {}
};

template<typename T>
struct Id<T, false>
{
    Int value;
    Id(Int v)
        : value(v)
    {
        if (v != 0)
        {
            throw std::runtime_error("Parser error: VoidId has non zero value. This action is supposed to return 0");
        }
    }
};

class Storage;
template<typename T>
struct IdAccess
{
    Storage&    storage;
    Int         index;
    IdAccess(Storage& storage, Id<T, IdTraits<T>::valid> id)
        : storage(storage)
        , index(id.value)
    {}

    ~IdAccess();

    Int reuse()
    {
        Int result = index;
        index = 0;
        return result;
    }

    using AccessType = typename IdTraits<T>::AccessType;
    operator T&() const;
};

using VoidId                = Id<Void, false>;
using IntId                 = Id<Int>;
using DeclListId            = Id<DeclList>;
using NamespaceListId       = Id<NamespaceList>;
using TypeListId            = Id<TypeList>;
using ExpressionListId      = Id<ExpressionList>;
using StatementListId       = Id<StatementList>;
using MemberInitListId      = Id<MemberInitList>;
using DeclId                = Id<Decl>;
using ScopeId               = Id<Scope>;
using CodeBlockId           = Id<CodeBlock>;
using MemberInitId          = Id<MemberInit>;
using NamespaceId           = Id<Namespace>;
using TypeId                = Id<Type>;
using ClassId               = Id<Class>;
using FunctionId            = Id<Function>;
using ObjectId              = Id<Object>;
using IdentifierId          = Id<Identifier>;
using StatementId           = Id<Statement>;
using ExpressionId          = Id<Expression>;

using IntAccess             = IdAccess<Int>;
using DeclListAccess        = IdAccess<DeclList>;
using NamespaceListAccess   = IdAccess<NamespaceList>;
using TypeListAccess        = IdAccess<TypeList>;
using ExpressionListAccess  = IdAccess<ExpressionList>;
using StatementListAccess   = IdAccess<StatementList>;
using MemberInitListAccess  = IdAccess<MemberInitList>;
using DeclAccess            = IdAccess<Decl>;
using ScopeAccess           = IdAccess<Scope>;
using CodeBlockAccess       = IdAccess<CodeBlock>;
using MemberInitAccess      = IdAccess<MemberInit>;
using NamespaceAccess       = IdAccess<Namespace>;
using TypeAccess            = IdAccess<Type>;
using ClassAccess           = IdAccess<Class>;
using FunctionAccess        = IdAccess<Function>;
using ObjectAccess          = IdAccess<Object>;
using IdentifierAccess      = IdAccess<Identifier>;
using StatementAccess       = IdAccess<Statement>;
using ExpressionAccess      = IdAccess<Expression>;

}

#endif
