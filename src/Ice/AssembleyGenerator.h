#ifndef THORSANVIL_ANVIL_ICE_ICECOMPILER_H
#define THORSANVIL_ANVIL_ICE_ICECOMPILER_H

#include "Declaration.h"
#include "Storage.h"
#include <string>
#include <iostream>

namespace ThorsAnvil::Anvil::Ice
{

class AssembleyGenerator
{
    NamespaceDecOrder&  namespaceDecOrder;
    Scope&              globalScope;
    std::ostream&       error;

    public:
        AssembleyGenerator(NamespaceDecOrder& namespaceDecOrder, Scope& scope, std::ostream& error);
        void generate(std::string const& entryPoint, std::ostream& output);

    private:
        void generate_InitStatement(std::ostream& output);
        void generate_CallsToConstructAllNameSpace(std::ostream& output);
        void generate_CallToEntryPoint(std::string const& entryPoint, std::ostream& output);
        void generate_CallsToDestructAllNameSpace(std::ostream& output);
        void generate_EndMainSection(std::ostream& output);
        void generate_AllNSCode(std::ostream& output);
        void generate_SpecificNSCodeBlock(std::ostream& output, Namespace& ns, std::string const& name);

};

}

#endif
