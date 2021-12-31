#ifndef THORSANVIL_ANVIL_ICE_STORAGE_ACCESS_H
#define THORSANVIL_ANVIL_ICE_STORAGE_ACCESS_H

#include "Common.h"

namespace ThorsAnvil::Anvil::Ice
{

class Storage;

template<typename T>
constexpr std::remove_reference_t<T>&& moveAccess(IdAccess<T>& access)
{
    return std::move(static_cast<T&>(access));
}
template<typename T>
constexpr std::remove_reference_t<T>&& moveAccess(IdAccess<T>&& access)
{
    return std::move(static_cast<T&>(access));
}

template<typename T>
struct IdTraits;

template<typename T, bool valid>
struct Id
{
    Int value;
    Id(Int v);
};

template<typename T>
struct Id<T, false>
{
    Int value;
    Id(Int v);
};

template<typename T>
struct IdAccess
{
    Storage&    storage;
    Int&        index;
    IdAccess(Storage& storage, Id<T, IdTraits<T>::valid>& id);
    ~IdAccess();

    Int reuse();

    using AccessType = typename IdTraits<T>::AccessType;
    operator T&() const;
};

}

#include "StorageAccess.tpp"

#endif
