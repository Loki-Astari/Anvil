#include "Declaration.h"

using namespace ThorsAnvil::Anvil::Ice;

std::pair<bool, NameRef> Scope::get(std::string const& name) const
{
    auto find = members.find(name);
    if (find == members.end())
    {
        return {false, find};
    }
    return {true, find};
}
