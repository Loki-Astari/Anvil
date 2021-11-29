#include "Assembler.h"
#include <sstream>
#include <iostream>

using namespace ThorsAnvil::Anvil::Fire;

Assembler::Assembler(std::istream& file, std::ostream& result, CodeBlock& memory)
    : error(false)
{
    std::string     line;
    while (std::getline(file, line))
    {
        auto count = assemble(line, result);
        memory.insert(std::end(memory), std::begin(instructions), std::begin(instructions) + count);
    }
}

int Assembler::assemble(std::string& line, std::ostream& result)
{
    // Remove comments.
    auto find = line.find("//");
    if (find != std::string::npos)
    {
        line.erase(find);
    }

    std::stringstream   lineStream(std::move(line));
    std::string         cmd;

    while (lineStream >> cmd)
    {
        if (cmd == "CMD")
        {
            return assembleCmd(lineStream, result);
        }
        if (cmd == "Load")
        {
            return assembleLoad(lineStream, result);
        }
    }
    return 0;
}


int Assembler::assembleCmd(std::stringstream& lineStream, std::ostream& result)
{
    std::string operation;
    lineStream >> operation;

    if (operation == "NoOp")
    {
        instructions[0] = Act_CMD | Cmd_NoOp;
        return 1;
    }
    else if (operation == "Kill")
    {
        Instruction value = 0;
        lineStream >> value;
        if (value <= Cmd_KillStatusMask)
        {
            instructions[0] = Act_CMD | Cmd_Kill | value;
            return 1;
        }
        result << "Invalid CMD Kill parameters: CMD Kill >" <<  value << "<\n";
        error = true;
        return 0;
    }
    else if (operation == "Init")
    {
        if ((lineStream >> instructions[0] >> instructions[1]) && ((instructions[0] & 0xFC00) == 0))
        {
            instructions[0] |= (Act_CMD | Cmd_Init);
            return 2;
        }
        result << "Invalid CMD Init parameters: CMD Init >" <<  instructions[0] << "< >" << instructions[1] << "<\n";
        error = true;
        return 0;
    }
    else if (operation == "Import")
    {
        std::string fileName;
        if (lineStream >> fileName)
        {
            auto pos = fileName.find_last_of('.');
            pos = pos == std::string::npos ? fileName.size() : pos;
            std::string extension = fileName.substr(pos);
            std::size_t fileNameSize = fileName.size();

            Instruction fileType;

            if (extension == ".fire")
            {
                fileNameSize -= 5;
                fileName[fileNameSize] = '\0';
                fileType = Cmd_ImportFileFire;
            }
            else
            {
                // If no extension assume the platform specific dll extension.
                fileType = Cmd_ImportFileDLL;
            }
            if (fileNameSize > 255)
            {
                result << "Invalid CMD Import: File Name to large >" << fileName << "<\n";
                error = true;
                return 0;
            }
            instructions[0] = Act_CMD | Cmd_Import | fileType | fileNameSize;
            int result = 1;
            for (std::size_t loop = 0; loop < fileNameSize; loop += 2)
            {
                instructions[1 + loop/2] = (static_cast<Instruction>(fileName.c_str()[loop]) << 8) | (static_cast<Instruction>(fileName.c_str()[loop + 1]));
                ++result;
            }
            return result;
        }
        result << "Invalid CMD Import: Could not read filename:\n";
        error = true;
        return 0;
    }
    else
    {
        result << "Invalid CMD Instruction: CMD >" << operation << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
}

bool Assembler::getRegisterName(Instruction& reg, int type, std::string const& value) const
{
    bool result = true;
    if (value == "Global")
    {
        reg  = RegGlobal[type];
    }
    else if (value == "Stack")
    {
        reg  = RegStack[type];
    }
    else if (value == "This")
    {
        reg  = RegThis[type];
    }
    else if (value == "Expr")
    {
        reg  = RegExpr[type];
    }
    else
    {
        result = false;
    }
    return result;
}

int Assembler::assembleLoad(std::stringstream& lineStream, std::ostream& result)
{
    std::string     dstRegValue;
    Instruction     dstReg;

    lineStream >> dstRegValue;
    if (!getRegisterName(dstReg, Assembler::Destination, dstRegValue))
    {
        result  << "Invalid Dst Register: Load\n"
                << ">" << dstRegValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    char    actionValue = 'X';

    lineStream >> actionValue;
    if (actionValue != '=' && actionValue != '*')
    {
        result  << "Invalid Action: Load\n"
                << dstRegValue << " >" << actionValue << "< " <<  lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }
    Instruction action = (actionValue == '=') ? Load_Set : Load_Read;

    std::string srcRegValue;
    Instruction srcReg = 0;

    lineStream >> srcRegValue;
    if (!getRegisterName(srcReg, Assembler::Source, srcRegValue))
    {
        result  << "Invalid Src Register: Load\n"
                << dstRegValue << " " << actionValue << " >" << srcRegValue << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    char    addition = 'X';

    lineStream >> addition;
    if (addition != '+')
    {
        result  << "Invalid Action Register: Load\n"
                << dstRegValue << " " << actionValue << " " << srcRegValue << " >" << addition << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    std::int32_t value;
    if (!(lineStream >> value))
    {
        result  << "Invalid Literal (Can't Read): Load\n"
                << dstRegValue << " " << actionValue << " " << srcRegValue << " " << addition << " >" << value << "< " << lineStream.rdbuf() << "<\n";
        error = true;
        return 0;
    }

    if (value < Load_MaxNeg || value > Load_MaxPos)
    {
        result  << "Invalid Literal (out of range): Load\n"
                << dstRegValue << " " << actionValue << " " << srcRegValue << " " << addition << " >" << value << "< " << lineStream.rdbuf() << "<\n";
        error = true;
        return 0;
    }


    if (value >= 0 && value <= Load_SmallMax)
    {
        instructions[0] = (Act_Load | dstReg | srcReg | action | Load_ValueSmall | (value & Load_ValueMask));
        return 1;
    }
    else
    {
        instructions[0] = (Act_Load | dstReg | srcReg | action | Load_ValueLarge | ((value >> 16) & Load_ValueMask));
        instructions[1] = value & 0xFFFF;
        return 2;
    }
}
