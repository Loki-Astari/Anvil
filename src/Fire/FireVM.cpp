#include "FireVM.h"
#include <string>

using namespace ThorsAnvil::Anvil::Fire;

FireVM::FireVM(std::size_t size)
    : global(10)
    , stack(size)
    , programCounter(0)
{}

void FireVM::load(std::istream& file)
{
    std::string line;
    while (std::getline(file, line))
    {

    }
}

void FireVM::run()
{
    bool running = true;
    for (programCounter = 0; running && programCounter < application.size(); ++programCounter)
    {
        Instruction& instruction = application[programCounter];
        Instruction  cmd = (instruction & cmdMask) >> cmdShift;
        //Instruction  reg = (instruction & regMask) >> regShift;
        //Instruction  off = (instruction & offMask) >> offShift;
        //Instruction  dat = (instruction & datMask) >> datShift;

        switch (cmd)
        {
        }
    }
}
