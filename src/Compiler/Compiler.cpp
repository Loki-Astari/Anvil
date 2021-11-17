#include "Compiler.h"

using namespace ThorsAnvil::Anvil::Ice;

Compiler::Compiler(std::string const& fileName)
    : file(fileName)
    , parser(*this, file)
{}

Compiler::~Compiler()
{
}

void Compiler::go()
{
    parser.parse();
}
