#ifndef THORSANVIL_ANVIL_FIRE_FIREVM_H
#define THORSANVIL_ANVIL_FIRE_FIREVM_H

#include "Memory.h"
#include "Util.h"

#include <map>
#include <vector>
#include <string>
#include <istream>

#include <cstdint>

namespace ThorsAnvil::Anvil::Fire
{

class FireVM
{
    // Data
    Frame           global;
    Frame           stack;
    CodeBlock&      memory;
    std::size_t     programCounter;

    public:
        FireVM(CodeBlock& memory);
        void run();
};

}

#endif
