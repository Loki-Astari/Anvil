#ifndef THORSANVIL_ANVIL_FIRE_ASSEMBLER_H
#define THORSANVIL_ANVIL_FIRE_ASSEMBLER_H

#include "Memory.h"

#include <map>
#include <vector>
#include <iostream>
#include <cstdint>

namespace ThorsAnvil::Anvil::Fire
{

using SymbolTable = std::map<std::string, std::int32_t>;

class Assembler
{
    bool            error;
    std::size_t     addr;
    std::ostream&   errorStream;
    SymbolTable&    stable;
    Instruction     instructions[5];

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
        void doLine(int count, std::function<void(Instruction)>&& action);

        int assemble(std::string& line, bool buildSymbols);
            int assemble_Cmd(std::istream& lineStream);
                int assemble_CmdNoOp();
                int assemble_CmdKill(std::istream& lineStream);
                int assemble_CmdInit(std::istream& lineStream);
            int assemble_Jump(std::istream& lineStream, bool BuildSymbols);
                int assemble_JumpReturn(std::istream& lineStream);
                int assemble_JumpJp(std::istream& lineStream, bool buildSymbols);
                int assemble_JumpCall(std::istream& lineStream, bool buildSymbols);
                int assemble_JumpMethod(std::istream& lineStream);

        std::uint32_t getAddress(std::string const& lineStream, bool buildSymbols);
        bool          getRegister(std::string const& addressRegValue);
        int           assemble_JumpLength(std::string const& cmd, std::string const& flagValue, std::istream& lineStream, bool buildSymbols);
        bool          assemble_JumpConditionFlag(std::string const& flagValue);

        std::string getAction(std::stringstream& lineStream, bool buildSymbols);

    public:
        // Actions
        static constexpr Instruction Action_Mask            = 0xF000;
        static constexpr Instruction Act_CMD                = 0x0000;
        static constexpr Instruction Act_Jump               = 0x1000;


        // Action CMD
        static constexpr Instruction Action_CMD_Mask        = 0x0E00;
        static constexpr Instruction Cmd_NoOp               = 0x0000;
        static constexpr Instruction Cmd_Kill               = 0x0200;
        static constexpr Instruction Cmd_Init               = 0x0400;
        static constexpr Instruction Cmd_InitShort          = 0x0400;
        static constexpr Instruction Cmd_InitLarge          = 0x0600;
        static constexpr Instruction Cmd_Import             = 0x0800;

        // Action CMD Kill
        static constexpr Instruction Cmd_Kill_Result_Mask   = 0x01FF;

        // Action CMD Init
        static constexpr Instruction Cmd_Init_GlobalMask    = 0x0200;
        static constexpr Instruction Cmd_Init_StackMask     = 0x0100;
        static constexpr Instruction Cmd_Init_Size_8_16     = 0x0000;
        static constexpr Instruction Cmd_Init_Size_8_32     = 0x0100;
        static constexpr Instruction Cmd_Init_Size_16_16    = 0x0200;
        static constexpr Instruction Cmd_Init_Size_16_32    = 0x0300;


        // Action Jump Type
        static constexpr Instruction Action_Jump_Mask       = 0x0E00;
        static constexpr Instruction Jump_Return            = 0x0000;
        static constexpr Instruction Jump_Method            = 0x0200;
        static constexpr Instruction Jump_Jp                = 0x0600;
        static constexpr Instruction Jump_Call              = 0x0A00;

        // Action Jump Masks/Shift
        static constexpr Instruction Action_JumpFlag_Mask   = 0x01C0;
        static constexpr Instruction Action_JumpReg_Mask    = 0x01C0;
        static constexpr int         Action_JumpFlag_Shift  = 6;
        static constexpr int         Action_JumpReg_Shift   = 6;

        // Action Jump Condition Flag
        static constexpr Instruction Jump_Condition_AL      = 0x0000;
        static constexpr Instruction Jump_Condition_EQ      = 0x0040;
        static constexpr Instruction Jump_Condition_GT      = 0x0080;
        static constexpr Instruction Jump_Condition_GE      = 0x00C0;
        static constexpr Instruction Jump_Condition_LT      = 0x0100;
        static constexpr Instruction Jump_Condition_LE      = 0x0140;
        static constexpr Instruction Jump_Condition_NE      = 0x0180;
        static constexpr Instruction Jump_Condition_XX      = 0x01C0;

        // Action Jump Register
        static constexpr Instruction Jump_Reg_Global        = 0x0000;
        static constexpr Instruction Jump_Reg_FramePointer  = 0x0040;
        static constexpr Instruction Jump_Reg_This          = 0x0080;
        static constexpr Instruction Jump_Reg_Extra         = 0x00C0;

        static constexpr Instruction Jump_Reg_StackPointer  = 0x0100;
        static constexpr Instruction Jump_Reg_Expr_1        = 0x0140;
        static constexpr Instruction Jump_Reg_Expr_2        = 0x0180;
        static constexpr Instruction Jump_Reg_Expr_3        = 0x01C0;

        // Action Jump Offset
        static constexpr Instruction Jump_Offset_Mask       = 0x003F;

        // Invalid Instructions
        static constexpr Instruction InvalidAction          = 0xFFFF;
        static constexpr Instruction InvalidCmd             = 0x0FFF;
        static constexpr Instruction InvalidJump            = 0x1FFF;
};

}

#endif
