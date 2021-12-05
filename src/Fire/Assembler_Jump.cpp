#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

std::uint32_t Assembler::getAddress(std::string const& destination, bool buildSymbols)
{
    // If there is not a valid destination then return an invalid address.
    if (destination == "")
    {
        return 0;
    }
    // If we are building symbols return the current address.
    // This will make sure relative jumps are in range for the first pass.
    if (buildSymbols)
    {
        return addr;
    }

    auto find = stable.find(destination);
    if (find != stable.end())
    {
        return find->second;
    }
    // We did not find the symbol return invalid address.
    return 0;
}

bool Assembler::getRegister(std::string const& addressRegValue)
{
    if (addressRegValue == "Global")            {instructions[0] |= Jump_Reg_Global;}
    else if (addressRegValue == "FramePointer") {instructions[0] |= Jump_Reg_FramePointer;}
    else if (addressRegValue == "This")         {instructions[0] |= Jump_Reg_This;}
    else if (addressRegValue == "Extra")        {instructions[0] |= Jump_Reg_Extra;}
    else if (addressRegValue == "StackPointer") {instructions[0] |= Jump_Reg_StackPointer;}
    else if (addressRegValue == "Expr-1")       {instructions[0] |= Jump_Reg_Expr_1;}
    else if (addressRegValue == "Expr-2")       {instructions[0] |= Jump_Reg_Expr_2;}
    else if (addressRegValue == "Expr-3")       {instructions[0] |= Jump_Reg_Expr_3;}
    else
    {
        return false;
    }
    return true;
}

bool Assembler::assemble_JumpConditionFlag(std::string const& flagValue)
{
    if (flagValue == "AL")          {instructions[0] |= Jump_Condition_AL;}
    else if (flagValue == "EQ")     {instructions[0] |= Jump_Condition_EQ;}
    else if (flagValue == "NE")     {instructions[0] |= Jump_Condition_NE;}
    else if (flagValue == "LT")     {instructions[0] |= Jump_Condition_LT;}
    else if (flagValue == "GT")     {instructions[0] |= Jump_Condition_GT;}
    else if (flagValue == "LE")     {instructions[0] |= Jump_Condition_LE;}
    else if (flagValue == "GE")     {instructions[0] |= Jump_Condition_GE;}
    else
    {
        return false;
    }
    return true;
}

int Assembler::assemble_JumpLength(std::string const& cmd, std::string const& flagValue, std::istream& lineStream, bool buildSymbols)
{
    std::string regValue;
    std::string jumpDestination;

    lineStream >> regValue >> jumpDestination;

    if (regValue == "Rel")
    {
        std::uint32_t destination = getAddress(jumpDestination, buildSymbols);
        std::int32_t  relative = destination - (addr + 2);

        if (destination != 0 && ((relative & 0xFFFF0000) == 0xFFFF0000 || (relative & 0xFFFF0000) == 0x00000000))
        {
            instructions[0] |= JumpSize_Rel;
            instructions[1] = static_cast<Instruction>(relative);
            return 2;
        }
    }
    else if (regValue == "Abs")
    {
        std::uint32_t destination = getAddress(jumpDestination, buildSymbols);

        if (destination != 0)
        {
            instructions[0] |= JumpSize_Abs;
            instructions[1] = (destination >> 16) & 0xFFFF;
            instructions[2] = (destination >> 0)  & 0xFFFF;
            return 3;
        }
    }
    else if (regValue == "Mem")
    {
        if (getRegister(jumpDestination))
        {
            instructions[0] |= JumpSize_Mem;
            return 1;
        }
    }
    else
    {
        errorStream << "Invalid Input: JUMP " << cmd << " " << flagValue << " >" << regValue << "< " << jumpDestination << " " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    errorStream << "Invalid Input: JUMP " << cmd << " " << flagValue << " " << regValue << " >" << jumpDestination << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}

int Assembler::assemble_Jump(std::istream& lineStream, bool buildSymbols)
{
    instructions[0] = Act_Jump;

    std::string     cmd;
    lineStream >> cmd;

    if (cmd == "Return")        {return assemble_JumpReturn(lineStream);}
    else if (cmd == "Call")     {return assemble_JumpCall(lineStream, buildSymbols);}
    else
    {
        // Unknown command report an error.
        errorStream << "Invalid Input: JUMP >" << cmd << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
}
int Assembler::assemble_JumpReturn(std::istream& lineStream)
{
    std::string     flagValue;
    lineStream >> flagValue;

    if (!assemble_JumpConditionFlag(flagValue))
    {
        errorStream << "Invalid Input: JUMP Return >" << flagValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    instructions[0] |= JumpType_Return;
    return 1;
}

int Assembler::assemble_JumpCall(std::istream& lineStream, bool buildSymbols)
{
    std::string     flagValue;
    lineStream >> flagValue;

    if (!assemble_JumpConditionFlag(flagValue))
    {
        errorStream << "Invalid Input: JUMP Call >" << flagValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    instructions[0] |= JumpType_Call;
    return assemble_JumpLength("Call", flagValue, lineStream, buildSymbols);
}
