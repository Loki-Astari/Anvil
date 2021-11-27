#include "Assembler.h"
#include <sstream>
#include <iostream>

using namespace ThorsAnvil::Anvil::Fire;

Assembler::Assembler(std::istream& file, std::ostream& result, std::vector<Instruction>& memory)
    : error(false)
{
    std::string     line;
    while (std::getline(file, line))
    {
        auto value = assemble(line, result);
        memory.insert(std::end(memory), std::begin(value), std::end(value));
    }
}

Assembler::Inst Assembler::assemble(std::string& line, std::ostream& result)
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
    }
    return {};
}


Assembler::Inst Assembler::assembleCmd(std::stringstream& lineStream, std::ostream& result)
{
    std::string operation;
    lineStream >> operation;

    if (operation == "NOOP")
    {
        return {Cmd_NoOp};
    }
    result << "Invalid Instruction: CMD " << operation << " " << lineStream.rdbuf();
    error = true;
    return {};
}
