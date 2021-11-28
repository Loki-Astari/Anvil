#ifndef THORSANVIL_ANVIL_FIRE_UTIL_H
#define THORSANVIL_ANVIL_FIRE_UTIL_H

#include <vector>

namespace ThorsAnvil::Anvil::Fire
{
    using Instruction   = std::uint16_t;
    using CodeBlock     = std::vector<Instruction>;
}

#endif
