#ifndef THORSANVIL_ANVIL_FIRE_ASSEMBLER_H
#define THORSANVIL_ANVIL_FIRE_ASSEMBLER_H

#include "Util.h"

#include <vector>
#include <string>
#include <istream>
#include <utility>
#include <initializer_list>


namespace ThorsAnvil::Anvil::Fire
{

class Assembler
{
    bool    error;
    public:
        Assembler(std::istream& file, std::ostream& message, std::vector<Instruction>& memory);

        bool isOK() const {return !error;}

    private:
        using Inst = std::initializer_list<Instruction>;
        Inst assemble(std::string& line, std::ostream& result);
        Inst assembleCmd(std::stringstream& lineStream, std::ostream& result);

        static constexpr Inst Cmd_NoOp       = {0x0000};
};

}

#endif
