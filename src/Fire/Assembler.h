#ifndef THORSANVIL_ANVIL_FIRE_ASSEMBLER_H
#define THORSANVIL_ANVIL_FIRE_ASSEMBLER_H

#include "Util.h"

#include <array>
#include <string>
#include <istream>
#include <utility>
#include <initializer_list>


namespace ThorsAnvil::Anvil::Fire
{

class Assembler
{
    bool            error;
    std::array<Instruction, 200>   instructions;
    public:
        Assembler(std::istream& file, std::ostream& message, CodeBlock& memory);

        bool isOK() const {return !error;}

    private:
        int  assemble(std::string& line, std::ostream& result);
        int  assembleCmd(std::stringstream& lineStream, std::ostream& result);
        int  assembleLoad(std::stringstream& lineStream, std::ostream& result);


        bool getRegisterName(Instruction& reg, int type, std::string const& value) const;
        bool getFromName(Instruction& reg, std::string const& value) const;

    public:
        static constexpr Instruction ActionMask         = 0xF000;
        static constexpr Instruction Act_CMD            = 0x0000;
        static constexpr Instruction Act_Load           = 0x1000;

        static constexpr Instruction Cmd_NoOp           = 0x0000;
        static constexpr Instruction Cmd_Kill           = 0x1000;
        static constexpr Instruction Cmd_Init           = 0x2000;
        static constexpr Instruction Cmd_Load           = 0x3000;

        static constexpr Instruction LoadFileFire       = 0x0000;
        static constexpr Instruction LoadFileDLL        = 0x0100;


        static constexpr int         Destination        = 0x0;
        static constexpr int         Source             = 0x1;

        static constexpr Instruction RegGlobal[2]       = {0x0000, 0x0000};
        static constexpr Instruction RegStack[2]        = {0x0400, 0x0040};
        static constexpr Instruction RegThis[2]         = {0x0800, 0x0080};
        static constexpr Instruction RegExpr[2]         = {0x0C00, 0x00C0};

        static constexpr Instruction MarkNeg            = 0x0040;

        static constexpr Instruction FromLiteral        = 0x0000;
        static constexpr Instruction FromRead           = 0x0100;
        static constexpr Instruction FromReg            = 0x0200;
        static constexpr Instruction FromIndRead        = 0x0300;

        static constexpr Instruction ValueRegMask       = 0x0200;   // 0 => Literal 1 => Register

        static constexpr Instruction ValueSmall         = 0x0000;
        static constexpr Instruction ValueLarge         = 0x0080;

        static constexpr int         ValueMaskSmall     = 0xFFFFFFE0;
        static constexpr int         ValueMaskLarge     = 0xFFE00000;
        static constexpr int         ValueMaskDataSmall = 0x0000001F;
};

}

#endif
