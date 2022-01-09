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
    private:
        void generateInitialize(std::ostream& output);
        void generateNameSpaceInit(std::ostream& output);
        void generateCallToApplication(std::ostream& output);
        void generateNameSpaceDeInit(std::ostream& output);
        void generateCallToQuitApp(std::ostream& output);


    public:
        // Visitor Pattern
        void outputNotImplementedYet(Decl const&, std::ostream& output);
};

}

#endif
