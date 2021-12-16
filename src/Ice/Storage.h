#ifndef THORSANVIL_ANVIL_ICE_STORAGE_H
#define THORSANVIL_ANVIL_ICE_STORAGE_H

#include "Action.h"     // Just to get Int
#include "Declaration/Types.h"
#include "Declaration/Objects.h"
#include "Declaration/Container.h"

#include <list>
#include <vector>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

using Data = std::variant<Int, TypeRef, ObjectRef, DeclRef, ScopeRef, ParamList, ObjectIdList, IdentifierList, ObjectId, std::string>;

template<typename T>
class StorageAccess;

class Storage
{
    std::vector<Data>   data;
    Int                 nextFree;

    public:
        Storage()
            : nextFree(0)
        {
            // We don't use location 0.
            // This is because ice.y parser passes back zero for no object returned.
            // So we don't want to confuse a no-object with an object.
            data.emplace_back(0);
        }

        template<typename T>
        Int add(T&& value)
        {
            if (nextFree == 0)
            {
                data.emplace_back(std::forward<T>(value));
                return data.size() - 1;
            }
            Int result = nextFree;
            nextFree = std::get<Int>(data[nextFree]);
            data[result] = std::move(value);
            return result;
        }

        void release(Int index)
        {
            if (index == 0)
            {
                throw std::runtime_error("Internal Compiler Error: Releasing Element Zero");
            }
            if (static_cast<std::size_t>(index) >= data.size())
            {
                throw std::runtime_error("Internal Compiler Error: Releasing Element beyond Range");
            }
            data[index] = nextFree;
            nextFree = index;
        }
    private:
        template<typename T> friend class StorageAccess;

        template<typename T>
        T& get(Int index)
        {
            if (index == 0)
            {
                throw std::runtime_error("Internal Compiler Error: Getting Element Zero");
            }
            if (static_cast<std::size_t>(index) >= data.size())
            {
                throw std::runtime_error("Internal Compiler Error: Getting Element beyond Range");
            }
            return std::get<T>(data[index]);
        }

};

template<typename T>
class StorageAccess
{
    Storage&    store;
    Int         index;
    T&          value;
    public:
        StorageAccess(Storage& store, Int& paramIndex)
            : store(store)
            , index(paramIndex)
            , value(store.get<T>(index))
        {
            paramIndex = 0;
        }
        ~StorageAccess()
        {
            if (index != 0)
            {
                store.release(index);
            }
        }
        Int reUse()
        {
            Int result = index;
            index = 0;
            return result;
        }
        T& getValue() const
        {
            return value;
        }
};
template<typename T>
class StorageAccessDirect: public StorageAccess<T>
{
    public:
        using StorageAccess<T>::StorageAccess;
        T& get() const
        {
            return StorageAccess<T>::getValue();
        }
        operator T& () const
        {
            return get();
        }
};
template<typename T>
class StorageAccessRef: public StorageAccess<std::reference_wrapper<T>>
{
    public:
        using StorageAccess<std::reference_wrapper<T>>::StorageAccess;
        T& get() const
        {
            return StorageAccess<std::reference_wrapper<T>>::getValue().get();
        }
        operator T& () const
        {
            return get();
        }
};


using SAccessInt            = StorageAccessDirect<Int>;
using SAccessType           = StorageAccessRef<Type>;
using SAccessObject         = StorageAccessRef<Object>;
using SAccessDecl           = StorageAccessRef<Decl>;
using SAccessScope          = StorageAccessRef<Scope>;
using SAccessParamList      = StorageAccessDirect<ParamList>;
using SAccessObjectIdList   = StorageAccessDirect<ObjectIdList>;
using SAccessIdentifierList = StorageAccessDirect<IdentifierList>;
using SAccessObjectId       = StorageAccessDirect<ObjectId>;
using SAccessString         = StorageAccessDirect<std::string>;

}

#endif
