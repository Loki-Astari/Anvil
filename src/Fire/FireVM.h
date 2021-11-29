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

enum Register {Global, Stack, This, Expr, MaxRegisters};

struct VMState
{
    VMState()
        : programCounter(0)
        , registerFrame{0, 0, 0, 0}
    {}
    Frame           global;
    Frame           stack;
    std::size_t     programCounter;
    std::size_t     registerFrame[MaxRegisters];
};

class FireVM
{
    VMState&        state;
    CodeBlock&      memory;
    int             result;
    bool            running;

    public:
        FireVM(VMState& state, CodeBlock& memory);
        int run();

    private:
        void runActionCMD(Instruction instruction);
        void runActionLoad(Instruction instruction);

        void runActionCmdKill(Instruction instruction);
        void runActionCmdInit(Instruction instruction);
        void runActionCmdImport(Instruction instruction);
};

}

#endif
