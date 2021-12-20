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
using Identifier            = std::string;


enum class ParserType {Void, NamespaceList, Namespace, Identifier};

template<ParserType PT>
struct IdTraits;

template<>
struct IdTraits<ParserType::Void>
{
    static constexpr bool valid = false;
};
template<>
struct IdTraits<ParserType::NamespaceList>
{
    static constexpr bool valid = true;
    using AccessType = NamespaceList;
    using ExportType = NamespaceList;
    static ExportType& convert(AccessType& object) {return object;}
    static NamespaceList defaultUnusedValue;
};
template<>
struct IdTraits<ParserType::Namespace>
{
    static constexpr bool valid = true;
    using AccessType = NamespaceRef;
    using ExportType = Namespace;
    static ExportType& convert(AccessType& object) {return object.get();}
    static Namespace defaultUnusedValue;
};
template<>
struct IdTraits<ParserType::Identifier>
{
    static constexpr bool valid = true;
    using AccessType = std::string;
    using ExportType = AccessType;
    static ExportType& convert(AccessType& object) {return object;}
    static std::string defaultUnusedValue;
};


template<ParserType PT, bool valid = true>
struct Id
{
    Int value;
    Id(Int v)
        : value(v)
    {}
};

template<ParserType PT>
struct Id<PT, false>
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
template<ParserType PT>
struct IdAccess
{
    Storage&    storage;
    Int         index;
    IdAccess(Storage& storage, Id<PT, IdTraits<PT>::valid> id)
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

    using AccessType = typename IdTraits<PT>::AccessType;
    using ExportType = typename IdTraits<PT>::ExportType;
    operator ExportType&() const;
};

using VoidId                = Id<ParserType::Void, false>;
using NamespaceListId       = Id<ParserType::NamespaceList>;
using NamespaceId           = Id<ParserType::Namespace>;
using IdentifierId          = Id<ParserType::Identifier>;

using NamespaceListAccess   = IdAccess<ParserType::NamespaceList>;
using NamespaceAccess       = IdAccess<ParserType::Namespace>;
using IdentifierAccess      = IdAccess<ParserType::Identifier>;

}

#endif
