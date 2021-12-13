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
    AllScopeAndName&    nameInfo;
    Scope&              globalScope;
    std::ostream&       error;

    public:
        AssembleyGenerator(AllScopeAndName& nameInfo, Scope& scope, std::ostream& error);
        void generate(std::string const& entryPoint, std::ostream& output);

    private:
        void generate_InitStatement(std::ostream& output);
        void generate_CallsToConstructAllNameSpace(std::ostream& output);
        void generate_CallToEntryPoint(std::string const& entryPoint, std::ostream& output);
        void generate_AllNSConstructors(std::ostream& output);

};

}

#endif
