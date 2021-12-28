#ifndef THORSANVIL_ANVIL_ICE_STORAGE_TPP
#define THORSANVIL_ANVIL_ICE_STORAGE_TPP

namespace ThorsAnvil::Anvil::Ice
{

template<typename T>
Int Storage::add(T&& value)
{
    if (nextFree == 0)
    {
        data.emplace_back(std::move(value));
        return data.size() - 1;
    }
    Int result = nextFree;
    nextFree = std::get<Int>(data[nextFree]);
    data[result] = std::move(value);
    return result;
}

template<typename T>
T& Storage::get(Int index)
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

}

#endif
