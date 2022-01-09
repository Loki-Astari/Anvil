#include "Generator.h"

#include <ostream>

using namespace ThorsAnvil::Anvil::Ice;

Generator::Generator(Scope& globalScope, NamespaceDecOrder& namespaceDecOrder)
    : globalScope(globalScope)
    , namespaceDecOrder(namespaceDecOrder)
{}

void Generator::generate(std::ostream& /*output*/)
{
}
