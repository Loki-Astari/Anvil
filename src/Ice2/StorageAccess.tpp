#ifndef THORSANVIL_ANVIL_ICE_PARSER_TYPES_TPP
#define THORSANVIL_ANVIL_ICE_PARSER_TYPES_TPP

#include "Declaration.h"
#include "Storage.h"

namespace ThorsAnvil::Anvil::Ice
{

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
struct IdTraits<TypeCList>
{
    static constexpr bool valid = true;
    static constexpr Int defaultStorageId = 6;
    using AccessType = TypeCList;
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


template<typename T, bool valid>
inline
Id<T, valid>::Id(Int v)
    : value(v)
{}

template<typename T>
inline
Id<T, false>::Id(Int v)
    : value(v)
{
    if (v != 0)
    {
        throw std::runtime_error("Parser error: VoidId has non zero value. This action is supposed to return 0");
    }
}

template<typename T>
inline
IdAccess<T>::IdAccess(Storage& storage, Id<T, IdTraits<T>::valid> id)
    : storage(storage)
    , index(id.value)
{}

template<typename T>
inline IdAccess<T>::~IdAccess()
{}

template<typename T>
IdAccess<T>::operator T&() const
{
    Int location = (index != 0) ? index : IdTraits<T>::defaultStorageId;
    return storage.get<AccessType>(location);
}

template<typename T>
inline
Int IdAccess<T>::reuse()
{
    Int result = index;
    index = 0;
    return result;
}

}

#endif
