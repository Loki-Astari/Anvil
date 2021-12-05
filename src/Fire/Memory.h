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
using Instruction   = std::int16_t;
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
            value = initValue;
        }
};

}

#endif
