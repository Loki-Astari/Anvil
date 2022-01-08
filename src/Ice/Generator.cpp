#include "Generator.h"
#include "Declaration.h"

#include <vector>
#include <iterator>
#include <numeric>

using namespace ThorsAnvil::Anvil::Ice;

Generator::Generator(Scope& globalScope, NamespaceDecOrder& namespaceDecOrder)
    : globalScope(globalScope)
    , namespaceDecOrder(namespaceDecOrder)
{}

void Generator::generate(std::ostream& /*output*/)
{
}
