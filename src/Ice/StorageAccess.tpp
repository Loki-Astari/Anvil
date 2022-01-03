#ifndef THORSANVIL_ANVIL_ICE_PARSER_TYPES_TPP
#define THORSANVIL_ANVIL_ICE_PARSER_TYPES_TPP

#include "Declaration.h"

namespace ThorsAnvil::Anvil::Ice
{

template<typename T>
struct IdTraits
{
    static constexpr bool valid = T::valid;
    using AccessType = std::reference_wrapper<T>;
};

template<>
struct IdTraits<Int>
{
    static constexpr bool valid = true;
    using AccessType = Int;
};
template<>
struct IdTraits<Identifier>
{
    static constexpr bool valid = true;
    using AccessType = std::string;
};
template<>
struct IdTraits<DeclList>
{
    static constexpr bool valid = true;
    using AccessType = DeclList;
};
template<>
struct IdTraits<NamespaceList>
{
    static constexpr bool valid = true;
    using AccessType = NamespaceList;
};
template<>
struct IdTraits<MemberInitList>
{
    static constexpr bool valid = true;
    using AccessType = MemberInitList;
};
template<>
struct IdTraits<TypeCList>
{
    static constexpr bool valid = true;
    using AccessType = TypeCList;
};
template<>
struct IdTraits<StatementList>
{
    static constexpr bool valid = true;
    using AccessType = StatementList;
};
template<>
struct IdTraits<ExpressionList>
{
    static constexpr bool valid = true;
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
IdAccess<T>::IdAccess(Storage& storage, Id<T, IdTraits<T>::valid>& id)
    : storage(storage)
    , index(id.value)
{}

template<typename T>
inline IdAccess<T>::~IdAccess()
{
    if (index != 0)
    {
        storage.release(index);
        index = 0;
    }
}

template<typename T>
IdAccess<T>::operator T&() const
{
    if (index == 0)
    {
        throw std::domain_error("Invalid storage index used");
    }
    return storage.get<AccessType>(index);
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
