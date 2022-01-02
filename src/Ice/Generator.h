#ifndef THORSANVIL_ANVIL_ICE_GENERATOR_H
#define THORSANVIL_ANVIL_ICE_GENERATOR_H

#include "Common.h"


namespace ThorsAnvil::Anvil::Ice
{

class Generator
{
    Scope&                  globalScope;
    NamespaceDecOrder&      namespaceDecOrder;

    public:
        Generator(Scope& globalScope, NamespaceDecOrder& namespaceDecOrder);

        void generate(std::ostream& output);
};

}

#endif
