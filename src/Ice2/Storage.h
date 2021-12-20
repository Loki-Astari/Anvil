#ifndef THORSANVIL_ANVIL_ICE_STORAGE_H
#define THORSANVIL_ANVIL_ICE_STORAGE_H

#include "ParserTypes.h"
#include "Declaration.h"

#include <vector>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

using Data = std::variant<Int, DeclList, NamespaceList, DeclRef, NamespaceRef, ClassRef, Identifier>;

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
            data.emplace_back(0UL);
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
    //private:
    //    template<typename T> friend class StorageAccess;

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

}

#endif
