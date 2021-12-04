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

        std::string getCommand(std::stringstream& lineStream, bool buildSymbols);
};

}

#endif
