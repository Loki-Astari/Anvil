#include "Storage.h"
#include "Declaration.h"
#include "Common.h"

using namespace ThorsAnvil::Anvil::Ice;

Storage::Storage()
    : nextFree(0)
{
    // We don't use location 0.
    // This is because ice.y parser passes back zero for no object returned.
    // So we don't want to confuse a no-object with an object.
    data.emplace_back(0UL);
}

void Storage::release(Int index)
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
