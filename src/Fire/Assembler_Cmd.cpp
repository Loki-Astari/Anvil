#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Cmd(std::istream& lineStream)
{
    instructions[0] = Act_CMD;

    std::string     cmd;
    lineStream >> cmd;

    if (cmd == "NoOp")      {return assemble_CmdNoOp();}
    if (cmd == "Kill")      {return assemble_CmdKill(lineStream);}
    if (cmd == "Init")      {return assemble_CmdInit(lineStream);}
    if (cmd == "Import")    {return assemble_CmdImport(lineStream);}

    // Unknown command report an error.
    errorStream << "Invalid Input: CMD >" << cmd << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}

int Assembler::assemble_CmdNoOp()
{
    instructions[0] |= Cmd_NoOp;
    return 1;
}

int Assembler::assemble_CmdKill(std::istream& lineStream)
{
    instructions[0] |= Cmd_Kill;

    std::int16_t  result;
    if (!(lineStream >> result && (result & Assembler::Cmd_Kill_Result_Mask) == result))
    {
        errorStream << "Invalid Input: CMD Kill >" << result << "< " << lineStream.rdbuf() << ": value out of range.\n";
        error = true;
        return 0;
    }
    instructions[0] |= result;

    return 1;
}

int Assembler::assemble_CmdInit(std::istream& lineStream)
{
    instructions[0] |= Cmd_Init;

    std::uint64_t    globalSize = 0xFFFF'FFFF'FFFF'FFFF;
    std::uint64_t    stackSize  = 0xFFFF'FFFF'FFFF'FFFF;

    lineStream >> globalSize >> stackSize;
    if (globalSize > 0xFFFF || stackSize > 0xFFFFFFFF)
    {
        errorStream << "Invalid Input: CMD Init >" << globalSize << " " << stackSize << "< " << lineStream.rdbuf() << ": size values out of range\n";
        error = true;
        return 0;
    }

    if (globalSize <= 0xFF && stackSize <= 0xFFFF)
    {
        instructions[0] |= Cmd_Init_Size_8_16 | (static_cast<Instruction>(globalSize & 0xFF));
        instructions[1] = static_cast<Instruction>(stackSize);
        return 2;
    }
    else if (globalSize <= 0xFF)
    {
        instructions[0] |= Cmd_Init_Size_8_32 | (static_cast<Instruction>(globalSize & 0xFF));
        instructions[1] = static_cast<Instruction>((stackSize >> 16) & 0xFFFF);
        instructions[2] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 3;
    }
    else if (stackSize <= 0xFFFF)
    {
        instructions[0] |= Cmd_Init_Size_16_16;
        instructions[1] = static_cast<Instruction>(globalSize & 0xFFFF);
        instructions[2] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 3;
    }
    else
    {
        instructions[0] |= Cmd_Init_Size_16_32;
        instructions[1] = static_cast<Instruction>(globalSize & 0xFFFF);
        instructions[2] = static_cast<Instruction>((stackSize >> 16) & 0xFFFF);
        instructions[3] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 4;
    }
}

int Assembler::assemble_CmdImport(std::istream& lineStream)
{
    instructions[0] |= Cmd_Import;

    std::string  importAction;
    lineStream >> importAction;
    if (importAction == "Load")             {return assemble_CmdImportLoad(lineStream);}
    else if (importAction == "GetSymbol")   {return assemble_CmdImportGetSymbol(lineStream);}
    else if (importAction == "Call")        {return assemble_CmdImportCall(lineStream);}
    else
    {
        errorStream << "Invalid Input: CMD Import >" << importAction << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
}

int Assembler::assemble_CmdImportGetRegisters(std::istream& lineStream, std::string const& importType, bool getReg2, bool getName)
{
    std::string regValue1;
    lineStream >> regValue1;
    if (!getRegister(regValue1, Cmd_Import_Reg1_Shift))
    {
        errorStream << "Invalid Input: CMD Import " << importType << " >" << regValue1 << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    std::string regValue2;
    if (getReg2)
    {
        lineStream >> regValue2;
        if (!getRegister(regValue2, Cmd_Import_Reg2_Shift))
        {
            errorStream << "Invalid Input: CMD Import " << importType << " " << regValue1 << " >" << regValue2 << "< " << lineStream.rdbuf() << "\n";
            error = true;
            return 0;
        }
    }

    if (getName)
    {
        std::string name;
        lineStream >> name;
        if (name == "")
        {
            errorStream << "Invalid Input: CMD Import " << importType << " " << regValue1 << " " << regValue2 << " ????: Name of object required.\n";
            error = true;
            return 0;
        }
        instructions[1] = name.size();
        std::size_t offset = name.size();
        offset += (offset % 2) == 1 ? 1 : 0;
        offset /= 2;
        instructions.resize(2 + offset);
        std::copy(std::begin(name), std::end(name), reinterpret_cast<char*>(&instructions[2]));
        return 2 + offset;
    }
    return 1;
}

int Assembler::assemble_CmdImportLoad(std::istream& lineStream)
{
    instructions[0] |= Cmd_Import_Load;
    return assemble_CmdImportGetRegisters(lineStream, "Load", false, true);

    // CMD Import Load Expr-1 Anvil.System
}
int Assembler::assemble_CmdImportGetSymbol(std::istream& lineStream)
{
    instructions[0] |= Cmd_Import_GetSymbol;
    return assemble_CmdImportGetRegisters(lineStream, "GetSymbol", true, true);

    //CMD Import GetSymbol Expr-1 Expr-2 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
}

int Assembler::assemble_CmdImportCall(std::istream& lineStream)
{
    instructions[0] |= Cmd_Import_Call;
    return assemble_CmdImportGetRegisters(lineStream, "Call", true, false);

    //CMD Import Call Expr-1 Expr-2
}
