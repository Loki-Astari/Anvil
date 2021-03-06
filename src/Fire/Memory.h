#ifndef THORSANVIL_ANVIL_FIRE_MEMORY_H
#define THORSANVIL_ANVIL_FIRE_MEMORY_H

#include <variant>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

namespace ThorsAnvil::Anvil::Fire
{

// Code Space representation
using Instruction   = std::uint16_t;
using CodeBlock     = std::vector<Instruction>;

// Data space representation
class MemoryLocation;

// Objects that can be stored in a memory location.
class Void {};
using CodeAddress   = std::size_t;
using DataAddress   = MemoryLocation*;
using DataFrame     = std::vector<MemoryLocation>;
using Int           = std::int32_t;
using String        = std::string;
using Array         = std::vector<DataFrame>;
using Map           = std::map<std::string, DataFrame>;

enum class MemoryType {Void, CodeAddress, DataAddress, DataFrame, Int, String, Array, Map};

struct MemoryLocationVisitorCheckType
{
    MemoryType operator()(Void&)           {return MemoryType::Void;}
    MemoryType operator()(CodeAddress&)    {return MemoryType::CodeAddress;}
    MemoryType operator()(DataAddress&)    {return MemoryType::DataAddress;}
    MemoryType operator()(DataFrame&)      {return MemoryType::DataFrame;}
    MemoryType operator()(Int&)            {return MemoryType::Int;}
    MemoryType operator()(String&)         {return MemoryType::String;}
    MemoryType operator()(Array&)          {return MemoryType::Array;}
    MemoryType operator()(Map&)            {return MemoryType::Map;}
};

class MemoryLocation
{
    using Object = std::variant<Void,
                                CodeAddress,
                                DataAddress,
                                DataFrame,
                                Int,                    // Std::Integer
                                String,                 // Std::String
                                Array,
                                Map
                               >;

    Object value;
    public:
        MemoryLocation();

        template<typename Visitor>
        auto visit(Visitor&& visitor)
        {
            return std::visit(std::move(visitor), value);
        }

        template<typename T>
        void init(T initValue)
        {
            value = std::move(initValue);
        }

        template<typename Type>
        Type& getValue(std::function<void()>&& action = [](){})
        {
            static Type defaultValue{};
            Type* check = std::get_if<Type>(&value);
            if (!check)
            {
                action();
            }
            return check ? *check : defaultValue;
        }
        MemoryLocation* getAddress(std::function<void()>&& action = [](){})
        {
            MemoryLocation** memory = std::get_if<DataAddress>(&value);
            if (memory != nullptr)
            {
                return *memory;
            }

            DataFrame* frame = std::get_if<DataFrame>(&value);
            if (frame != nullptr)
            {
                return &(*frame)[0];
            }
            action();
            return nullptr;
        }
};

}

#endif
