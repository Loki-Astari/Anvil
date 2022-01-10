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
        void outputNotImplementedYet(Decl const& decl, std::ostream& output);
        void outputNamespace(Namespace const& ns, std::ostream& output);
        void outputFunction(ObjectFunction const& func, std::ostream& output);
        void outputCodeBlock(StatementCodeBlock const& func, std::ostream& output);
};

}

#endif
