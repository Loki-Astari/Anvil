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
        // Can't have 0 as a value
        return addr == 0 ? 1 : addr;
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

int Assembler::assemble_Jump(std::istream& lineStream, bool buildSymbols)
{
    instructions[0] = Act_Jump;

    std::string action;
    lineStream >> action;

    if (action == "Return")         {return assemble_JumpReturn(lineStream);}
    else if (action == "Jp")        {return assemble_JumpJp(lineStream, buildSymbols);}
    else if (action == "Call")      {return assemble_JumpCall(lineStream, buildSymbols);}
    else if (action == "Method")    {return assemble_JumpMethod(lineStream);}

    errorStream << "Invalid Input: JUMP >" << action << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}
int Assembler::assemble_JumpReturn(std::istream& lineStream)
{
    std::string flagValue;
    lineStream >> flagValue;
    if (!assemble_JumpConditionFlag(flagValue))
    {
        errorStream << "Invalid Input: JUMP Return >" << flagValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    instructions[0] |= Jump_Return;
    return 1;
}

int Assembler::assemble_JumpJp(std::istream& lineStream, bool buildSymbols)
{
    std::string flagValue;
    lineStream >> flagValue;
    if (!assemble_JumpConditionFlag(flagValue))
    {
        errorStream << "Invalid Input: JUMP Jp >" << flagValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    std::string destination;
    lineStream >> destination;
    std::uint32_t dest = getAddress(destination, buildSymbols);
    if (dest == 0)
    {
        errorStream << "Invalid Input: JUMP Jp " << flagValue << " >" << destination << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    std::uint32_t addFrom  = addr + 2;
    std::uint32_t relative = dest > addFrom ? dest - addFrom : addFrom - dest;
    if (relative > 0x1F'FFFF)
    {
        errorStream << "Invalid Input: JUMP Jp " << flagValue << " >" << destination << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    std::int32_t offset = relative;
    std::int32_t jump = (dest > addr ? offset : -offset);

    instructions[0] |= Jump_Jp | ((jump >> 16) & 0x3F);
    instructions[1] = jump & 0xFFFF;
    return 2;
}

int Assembler::assemble_JumpCall(std::istream& lineStream, bool buildSymbols)
{
    std::string regValue;
    lineStream >> regValue;
    if (!getRegister(regValue))
    {
        errorStream << "Invalid Input: JUMP Call >" << regValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    std::string destination;
    lineStream >> destination;
    std::uint32_t dest = getAddress(destination, buildSymbols);
    if (dest == 0)
    {
        errorStream << "Invalid Input: JUMP Call " << regValue << " >" << destination << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    instructions[0] |= Jump_Call;
    instructions[1] = (dest >> 16) & 0xFFFF;
    instructions[2] = dest & 0xFFFF;
    return 3;
}

int Assembler::assemble_JumpMethod(std::istream& lineStream)
{
    std::string regValue;
    lineStream >> regValue;
    if (!getRegister(regValue))
    {
        errorStream << "Invalid Input: JUMP Method >" << regValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    std::uint16_t offset = 0x0FFFF;
    lineStream >> offset;
    if (offset > 0x003F)
    {
        errorStream << "Invalid Input: JUMP Method " << regValue << " >" << offset << "< " << lineStream.rdbuf() << ": Out of range\n";
        error = true;
        return 0;
    }

    instructions[0] |= Jump_Method | (offset & 0x3F);
    return 1;
}
