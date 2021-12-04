#include "Assembler.h"
#include <sstream>
#include <iostream>

using namespace ThorsAnvil::Anvil::Fire;

Assembler::Assembler(std::ostream& errorStream, SymbolTable& stable)
    : error(false)
    , addr(0)
    , output(errorStream)
    , stable(stable)
{}

void Assembler::assemble(std::istream& input, CodeBlock& output)
{
    firstPass(input);
    doPass(input, [&output](Instruction inst){output.emplace_back(inst);}, false);
}

void Assembler::assemble(std::istream& input, std::ostream& output)
{
    firstPass(input);
    doPass(input, [&output](Instruction inst){output.write(reinterpret_cast<char*>(&inst), 2);}, false);
}

void Assembler::firstPass(std::istream& input)
{
    // Get the current position.
    // After the first pass we want to reset for the second.
    std::streampos  from = input.tellg();
    doPass(input, [](Instruction){}, true);

    if (!error)
    {
        input.clear();
        input.seekg(from, std::ios_base::beg);
    }
}

bool Assembler::isOK() const
{
    return !error;
}

void Assembler::doPass(std::istream& input, std::function<void(Instruction)>&& action, bool buildSymbols)
{
    std::string     line;
    addr = 0;
    while (std::getline(input, line))
    {
        auto count = assemble(line, buildSymbols);
        for (int loop = 0; loop < count; ++loop)
        {
            action(instructions[loop]);
            ++addr;
        }
    }
}

int Assembler::assemble(std::string& line, bool buildSymbols)
{
    // Remove comments.
    auto find = line.find("//");
    if (find != std::string::npos)
    {
        line.erase(find);
    }

    std::stringstream   lineStream(std::move(line));
    std::string         cmd = getCommand(lineStream, buildSymbols);

    // Check for valid commands.
    if (cmd == "")
    {
        // Ignore Empty lines
        return 0;
    }
    if (cmd == "CMD")
    {
        return assemble_Cmd(lineStream);
    }

    // Unknown command report an error.
    output << "Invalid Input: >" << cmd << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}

std::string Assembler::getCommand(std::stringstream& lineStream, bool buildSymbols)
{
    std::string cmd;
    while (lineStream >> cmd)
    {
        while (cmd.size() != 0)
        {
            // Check for a label.
            auto pos = cmd.find(":");
            if (pos == std::string::npos)
            {
                return cmd;
            }

            // We found a label so record it.
            if (buildSymbols)
            {
                stable[cmd.substr(0, pos)] = addr;
            }

            // Remove label from line.
            if (pos + 1 == cmd.size())
            {
                // If there is no text after the colon
                // we need to read the next word from
                // the stream.
                cmd.clear();
            }
            else
            {
                // The label was immediately followed by text.
                // Remove the label we just processed and
                // repeat the processes.
                cmd = cmd.substr(pos+1);
            }
        }
    }
    // Valid commands are returned inside the loop.
    return "";
}
