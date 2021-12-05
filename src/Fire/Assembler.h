#ifndef THORSANVIL_ANVIL_FIRE_ASSEMBLER_H
#define THORSANVIL_ANVIL_FIRE_ASSEMBLER_H

#include <map>
#include <vector>
#include <iostream>
#include <cstdint>

namespace ThorsAnvil::Anvil::Fire
{

using Instruction = std::int16_t;
using CodeBlock = std::vector<Instruction>;
using SymbolTable = std::map<std::string, std::int32_t>;

class Assembler
{
    bool            error;
    std::size_t     addr;
    std::ostream&   output;
    SymbolTable&    stable;
    Instruction     instructions[4];

    public:
        Assembler(std::ostream& errorStream, SymbolTable& stable);

        void assemble(std::istream& input, CodeBlock& output);
        void assemble(std::istream& input, std::ostream& output);

        bool isOK() const;
    private:
        void firstPass(std::istream& input);
        void secondPass(std::istream& input, CodeBlock& output);
        void secondPass(std::istream& input, std::ostream& output);

        void doPass(std::istream& input, std::function<void(Instruction)>&& action, bool buildSymbols);

        int assemble(std::string& line, bool buildSymbols);
            int assemble_Cmd(std::istream& lineStream);
                int assemble_CmdNoOp(std::istream& lineStream);
                int assemble_CmdKill(std::istream& lineStream);

        std::string getAction(std::stringstream& lineStream, bool buildSymbols);

    public:
        // Actions
        static constexpr Instruction ActionMask     = 0xF000;
        static constexpr Instruction Act_CMD        = 0x0000;


        // CMD
        static constexpr Instruction ActionCMDMask  = 0x0C00;
        static constexpr Instruction Cmd_NoOp       = 0x0000;
        static constexpr Instruction Cmd_Kill       = 0x0400;

        // CMD Kill
        static constexpr Instruction Cmd_ResultMask = 0x03FF;

        // Invalid Instructions
        static constexpr Instruction InvalidAction  = 0xFFFF;
        static constexpr Instruction InvalidCmd     = 0x0FFF;
};

}

#endif
