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
    Frame               global;
    Frame               stack;

    // See: MemoryLocation::Object      in Memory.h
    // The DataTypes is part of the instruction.
    // If the instruction interacts with memory then it must
    // specify the correct type of the memory location.
    // If it does not we will generate an error.
    enum DataTypes    { Int,
                        String,
                        // Arrays
                        ArrayInt, ArrayString, ArrayFrame,
                        // Map Int
                        MapInt2Int, MapInt2String, MapInt2Frame,
                        // Map String
                        MapString2Int, MapString2String, MapString2Frame,
                        Frame,
                        Code
                      }; // Currently 13. We reserve 4 bits (see dat below) so can go to 16
    // Memory Registers
    enum DataRegisters  {Global, Stack, This, Expression, MaxRegistersCount};
    FrameView   registers[static_cast<int>(MaxRegistersCount)];
    // Flag Register
    // enum Flags          {Always, Less, LessEqual, Equal, GreatEqual, Great};
    // int         flags;

    // Application
    static constexpr Instruction cmdMask    = 0xF000;   // 16 instructions
    static constexpr Instruction cmdShift   = 12;
    static constexpr Instruction regMask    = 0x0C00;   // 4 registers
    static constexpr Instruction regShift   = 10;
    static constexpr Instruction offMask    = 0x03F0;   // 256 offset into frame.
    static constexpr Instruction offShift   = 4;
    static constexpr Instruction datMask    = 0x000F;   // 16 data values.
    static constexpr Instruction datShift   = 0;
    std::vector<Instruction>    application;    // Code Memory

    std::size_t         programCounter;

    public:
        FireVM(std::size_t size);
        void load(std::istream& file);
        void run();
};

}

#endif
