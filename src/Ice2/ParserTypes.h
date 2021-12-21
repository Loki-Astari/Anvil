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
using Identifier            = std::string;

template<typename T>
struct IdTraits
{
    static constexpr bool valid = T::valid;
    static constexpr Int defaultStorageId = T::defaultStorageId;
    using AccessType = std::reference_wrapper<T>;
};

template<>
struct IdTraits<DeclList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 1;
    using AccessType = DeclList;
};
template<>
struct IdTraits<NamespaceList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 2;
    using AccessType = NamespaceList;
};
template<>
struct IdTraits<Identifier>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 6;
    using AccessType = std::string;
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
using DeclListId            = Id<DeclList>;
using NamespaceListId       = Id<NamespaceList>;
using DeclId                = Id<Decl>;
using NamespaceId           = Id<Namespace>;
using ClassId               = Id<Class>;
using IdentifierId          = Id<Identifier>;

using DeclListAccess        = IdAccess<DeclList>;
using NamespaceListAccess   = IdAccess<NamespaceList>;
using DeclAccess            = IdAccess<Decl>;
using NamespaceAccess       = IdAccess<Namespace>;
using ClassAccess           = IdAccess<Class>;
using IdentifierAccess      = IdAccess<Identifier>;

}

#endif
