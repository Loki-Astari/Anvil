#include "Compiler.h"

using namespace ThorsAnvil::Anvil::Ice;

Type::~Type()
{}

Compiler::Compiler(std::string const& fileName)
    : file(fileName)
    , parser(*this, file)
{
    currentScope.push_back(std::cref(static_cast<Scope&>(globalScope)));
}

Compiler::~Compiler()
{
}

void Compiler::go()
{
    parser.parse();
}
