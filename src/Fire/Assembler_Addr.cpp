#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Addr(std::istream& lineStream, bool buildSymbols)
{
    instructions[0] = Act_Addr;

    std::string action;
    lineStream >> action;

    if (action == "INC")        {return assemble_AddrINC(lineStream, Addr_INC, action);}
    else if (action == "DEC")   {return assemble_AddrINC(lineStream, Addr_DEC, action);}
    else if (action == "LRR")   {return assemble_AddrAssign(lineStream,Addr_LRR, action);}
    else if (action == "LRP")   {return assemble_AddrAssign(lineStream,Addr_LRP, action);}
    else if (action == "LMR")   {return assemble_AddrAssign(lineStream,Addr_LMR, action);}
    else if (action == "LMP")   {return assemble_AddrAssign(lineStream,Addr_LMP, action);}
    else if (action == "LML")   {return assemble_AddrLiteral(lineStream,Addr_LML, action, buildSymbols);}
    else
    {
        errorStream << "Invalid Input: ADDR >" << action << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
}

int Assembler::assemble_AddrINC(std::istream& lineStream, Instruction flag, std::string const& action)
{
    instructions[0] |= flag;

    std::string regValue1;
    lineStream >> regValue1;
    if (!getRegister(regValue1, Addr_Reg1_Shift))
    {
        errorStream << "Invalid Input: ADDR " << action << " >" << regValue1 << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    Instruction offset = -1;
    if (!((lineStream >> offset) && offset >= 0 && offset <= 0x003F))
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " >" << offset << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    instructions[0] |= offset;
    return 1;
}

int Assembler::assemble_AddrAssign(std::istream& lineStream, Instruction flag, std::string const& action)
{
    std::string regValue1 = assemble_AddrLHS(lineStream, flag, action);
    if (regValue1 == "")
    {
        return 0;
    }
    return assemble_AddrRHS(lineStream, action, regValue1);
}

int Assembler::assemble_AddrLiteral(std::istream& lineStream, Instruction flag, std::string const& action, bool buildSymbol)
{
    std::string regValue1 = assemble_AddrLHS(lineStream, flag, action);
    if (regValue1 == "")
    {
        return 0;
    }
    std::string literal;
    lineStream >> literal;
    if (!assemble_AddrGetLiteralType(literal))
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " = >" << literal << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    switch (Addr_Literal_Mask & instructions[0])
    {
        case Addr_Literal_CodeAddr:
        {
            std::string destination;
            lineStream >> destination;
            std::uint32_t addr = getAddress(destination, buildSymbol);
            if (addr == 0)
            {
                errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " = " << literal << " >" << destination << "< " << lineStream.rdbuf() << "\n";
                error = true;
                return 0;
            }
            instructions[1] = static_cast<Instruction>((addr >> 16) & 0xFFFF);
            instructions[2] = static_cast<Instruction>(addr & 0xFFFF);
            return 3;
        }
        case Addr_Literal_DataFrame:
        {
            Instruction size = 0;
            lineStream >> size;
            instructions[1] = size;
            return 2;
        }
        case Addr_Literal_Int:
        {
            std::int32_t value;
            lineStream >> value;
            instructions[1] = static_cast<Instruction>((value >> 16) & 0xFFFF);
            instructions[2] = static_cast<Instruction>(value & 0xFFFF);
            return 3;
        }
        case Addr_Literal_String:
        {
            for (int x = lineStream.peek(); x != EOF && std::isspace(x);)
            {
                lineStream.get();
                x = lineStream.peek();
            }
            std::string line;
            std::getline(lineStream, line);
            instructions[1] = static_cast<Instruction>(line.size());
            // Get the string size.
            std::size_t offset = line.size();
            // Make it even (As we are coying into space that is blocks of 16bits.
            offset = offset + ((offset % 2) == 1 ? 1 : 0);
            // Total size: Instruction + Size + String length
            offset = 2 + (offset / 2);
            instructions.resize(offset);
            std::copy(std::begin(line), std::end(line), reinterpret_cast<char*>(&instructions[2]));
            return offset;
        }
    }
    return 0;
}

std::string Assembler::assemble_AddrLHS(std::istream& lineStream, Instruction flag, std::string const& action)
{
    instructions[0] |= flag;

    std::string regValue1;
    lineStream >> regValue1;
    if (!getRegister(regValue1, Addr_Reg1_Shift))
    {
        errorStream << "Invalid Input: ADDR " << action << " >" << regValue1 << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return "";
    }

    char value = 'X';
    lineStream >> value;
    if (value != '=')
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " >" << value << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return "";
    }
    return regValue1;
}

int Assembler::assemble_AddrRHS(std::istream& lineStream, std::string const& action, std::string const& regValue1)
{
    std::string regValue2;
    lineStream >> regValue2;
    if (!getRegister(regValue2, Addr_Reg2_Shift))
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " = >" << regValue2 << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    char value = 'X';
    lineStream >> value;
    if (value != '+')
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " = " << regValue2 << " >" << value << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    std::int32_t offset = -1;
    if (!((lineStream >> offset) && offset >= 0 && offset <= 0xFFFF))
    {
        errorStream << "Invalid Input: ADDR " << action << " " << regValue1 << " = " << regValue2 << " + >" << offset << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    if (offset == 0)
    {
        return 1;
    }
    instructions[0] |= Addr_AddType_Value;
    instructions[1] = static_cast<Instruction>(offset);
    return 2;
}

bool Assembler::assemble_AddrGetLiteralType(std::string const& type)
{
    if (type == "CodeAddress")          {instructions[0] |= Addr_Literal_CodeAddr;}
    else if (type == "DataFrame")       {instructions[0] |= Addr_Literal_DataFrame;}
    else if (type == "Int")             {instructions[0] |= Addr_Literal_Int;}
    else if (type == "String")          {instructions[0] |= Addr_Literal_String;}
    else
    {
        return false;
    }
    return true;
}
