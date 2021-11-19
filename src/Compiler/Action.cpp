#include "Action.h"

#include <iostream>

using namespace ThorsAnvil::Anvil::Ice;

Action::Action(std::ostream& output)
    : output(output)
{}

Action::~Action()
{}

void Action::log(char const* msg)
{
    output << msg << "\n";
}
