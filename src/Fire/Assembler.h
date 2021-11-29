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
        // See Instruction.help

        // Action: Mask and Values.
        static constexpr Instruction ActionMask                 = 0xF000;
        static constexpr Instruction Act_CMD                    = 0x0000;
        static constexpr Instruction Act_Load                   = 0x1000;
        // TODO Add More

        // Action: CMD: CMD Mask and values.
            static constexpr Instruction ActionCMDMask          = 0x0C00;
            static constexpr Instruction Cmd_NoOp               = 0x0000;
            static constexpr Instruction Cmd_Kill               = 0x0400;
            static constexpr Instruction Cmd_Init               = 0x0800;
            static constexpr Instruction Cmd_Import             = 0x0C00;

            // Action: CMD: CMD: NoOp

            // Action: CMD: CMD: Kill value Mask.
            static constexpr Instruction Cmd_KillStatusMask     = 0x03FF;

            // Action: CMD: CMD: Init value Mask.
            static constexpr Instruction Cmd_InitGlobalMask     = 0x03FF;

            // Action: CMD: CMD: Load File Mask Value
            static constexpr Instruction Cmd_ImportFileMask     = 0x0300;
            static constexpr Instruction Cmd_ImportFileFire     = 0x0000;
            static constexpr Instruction Cmd_ImportFileDLL      = 0x0100;
            // Action: CMD: CMD: Load File Size Mask
            static constexpr Instruction Cmd_ImportSizeMask     = 0x00FF;


        // Action: Load: Load Mask(s)
            static constexpr Instruction Load_DstRegMask        = 0x0C00;
            static constexpr Instruction Load_FromMask          = 0x0300;
            static constexpr Instruction Load_SrcRegMask        = 0x00C0; // Also used for Typ field
            static constexpr Instruction Load_SizeMask          = 0x0020;
            static constexpr Instruction Load_ValueMask         = 0x001F;

            // Action: Load: Register Info (Load_DstRegMask, Load_SrcRegMask)
            static constexpr Instruction RegGlobal[2]           = {0x0000, 0x0000};
            static constexpr Instruction RegStack[2]            = {0x0400, 0x0040};
            static constexpr Instruction RegThis[2]             = {0x0800, 0x0080};
            static constexpr Instruction RegExpr[2]             = {0x0C00, 0x00C0};

            static constexpr int         Destination            = 0;
            static constexpr int         Source                 = 1;
            static constexpr int         DestinationShift       = 10;
            static constexpr int         SourceShift            = 6;

            // Action: Load: From Mask and Value (Load_FromMask)
            static constexpr Instruction Load_FromLiteral       = 0x0000;
            static constexpr Instruction Load_FromRead          = 0x0100;
            static constexpr Instruction Load_FromReg           = 0x0200;
            static constexpr Instruction Load_FromIndRead       = 0x0300;

                // Action: Load From get sub information about the From Field.
                static constexpr Instruction Load_ValueRegMask  = 0x0200;   // 0 => Literal 1 => Register
                static constexpr Instruction Load_ValueIndMask  = 0x0100;   // 0 => Value   1 => Indirect (read from memory)

            // Action: Load: Literal Typ Values (uses Load_SizeMask)
            static constexpr Instruction Load_MarkNeg           = 0x0040;
            static constexpr Instruction Load_MarkRel           = 0x0080;

            // Action: Load: Size Values (Load_SizeMask)
            static constexpr Instruction Load_ValueSmall        = 0x0000;
            static constexpr Instruction Load_ValueLarge        = 0x0020;

            // Action: Load Value (Load_ValueMask)
            static constexpr int         Load_ValueMaskSmall    = 0xFFFFFFE0;   // Check if assembly literal fits into small value
            static constexpr int         Load_ValueMaskLarge    = 0xFFE00000;   // Check if assembly literal fits into large value
            static constexpr int         Load_ValueMaskDataSmall= 0x0000001F;
};

}

#endif
