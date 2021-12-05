#include "Assembler.h"
#include <sstream>
#include <iostream>

using namespace ThorsAnvil::Anvil::Fire;

Assembler::Assembler(std::ostream& errorStream, SymbolTable& stable)
    : error(false)
    , addr(0)
    , errorStream(errorStream)
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
    std::string         action  = getAction(lineStream, buildSymbols);
    int                 length  = 0;

    // Check for valid commands.
    if (action == "")
    {
        // Ignore Empty lines
    }
    else if (action == "CMD")
    {
        length = assemble_Cmd(lineStream);
    }
    else if (action == "JUMP")
    {
        length = assemble_Jump(lineStream, buildSymbols);
    }
    else
    {
        // Unknown command report an error.
        errorStream << "Invalid Input: >" << action << "< " << lineStream.rdbuf() << "\n";
        error = true;
        return 0;
    }

    // Check for an unconsumed input on the line
    char invalid;
    if (lineStream >> invalid)
    {
        std::string badInput;
        std::getline(lineStream, badInput);
        badInput.insert(0, 1, invalid);

        auto find = line.find(badInput);
        for (auto lastFind = find; lastFind < line.size(); lastFind = line.find(badInput, lastFind + 1))
        {
            find = lastFind;
        }

        errorStream << "Invalid Input: " << line.substr(0, find) << " >" << badInput << "<\n";
        error = true;
        return 0;
    }
    return length;
}

std::string Assembler::getAction(std::stringstream& lineStream, bool buildSymbols)
{
    std::string action;
    while (lineStream >> action)
    {
        while (action.size() != 0)
        {
            // Check for a label.
            auto pos = action.find(":");
            if (pos == std::string::npos)
            {
                return action;
            }

            if (pos == 0)
            {
                errorStream << "Invalid Input: Bad Label\n";
                error = true;
                return "";
            }

            // We found a label so record it.
            if (buildSymbols)
            {
                stable[action.substr(0, pos)] = addr;
            }

            // Remove label from line.
            if (pos + 1 == action.size())
            {
                // If there is no text after the colon
                // we need to read the next word from
                // the stream.
                action.clear();
            }
            else
            {
                // The label was immediately followed by text.
                // Remove the label we just processed and
                // repeat the processes.
                action = action.substr(pos+1);
            }
        }
    }
    // Valid commands are returned inside the loop.
    return "";
}
