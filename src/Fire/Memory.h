#ifndef THORSANVIL_ANVIL_FIRE_MEMORY_H
#define THORSANVIL_ANVIL_FIRE_MEMORY_H

#include <variant>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

namespace ThorsAnvil::Anvil::Fire
{

class MemoryLocation;

class Dead
{};

using Code = std::size_t;
using Frame = std::vector<MemoryLocation>;

class FrameView
{
    MemoryLocation*     b;
    MemoryLocation*     e;

    public:
        FrameView();
        FrameView(Frame& frame, std::size_t start, std::size_t size);

        FrameView(FrameView const&)             = default;
        FrameView(FrameView&&)                  = default;
        FrameView& operator=(FrameView const&)  = default;
        FrameView& operator=(FrameView&&)       = default;

        MemoryLocation* begin() const   {return b;}
        MemoryLocation* end()   const   {return e;}
};


class MemoryLocation
{
    using Object = std::variant<Dead,
                                std::int32_t,               // Std::Integer
                                std::string,                // Std::String
                                // Array
                                std::vector<std::int32_t>,
                                std::vector<std::string>,
                                std::vector<Frame>,
                                // Map
                                std::map<std::int32_t, std::int32_t>,
                                std::map<std::int32_t, std::string>,
                                std::map<std::int32_t, Frame>,
                                std::map<std::string,  std::int32_t>,
                                std::map<std::string,  std::string>,
                                std::map<std::string,  Frame>,
                                // Class
                                Frame,
                                // Address of function.
                                Code
                               >;

    Object value;
    public:
        MemoryLocation();

        template<typename T>
        MemoryLocation& operator=(T const& v)
        {
            value = v;
            return *this;
        }
        template<typename T>
        MemoryLocation& operator=(T&& v)
        {
            value = std::move(v);
            return *this;
        }
};

}

#endif
