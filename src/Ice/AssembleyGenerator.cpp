#include "AssembleyGenerator.h"

#include "Lexer.h"
#include "Semantic.h"
#include "Parser.h"

#include <numeric>

using namespace ThorsAnvil::Anvil::Ice;

AssembleyGenerator::AssembleyGenerator(AllScopeAndName& nameInfo, Scope& scope, std::ostream& error)
    : nameInfo(nameInfo)
    , globalScope(scope)
    , error(error)
{}

void AssembleyGenerator::generate(std::string const& entryPoint, std::ostream& output)
{
    generate_InitStatement(output);
    generate_CallsToConstructAllNameSpace(output);
    generate_CallToEntryPoint(entryPoint, output);
    generate_AllNSConstructors(output);
}

void AssembleyGenerator::generate_InitStatement(std::ostream& output)
{
    std::size_t globalSize = std::accumulate(std::begin(nameInfo), std::end(nameInfo), std::size_t{0},
                                                [](std::size_t lhs, auto const& rhs)
                                                {
                                                    return lhs + rhs.first.get().size();
                                                }
                                            );
    output <<   "// Initialize the Global Size\n"
           <<   "CMD Init " << globalSize << " " << 10000 << "\n";
}

void AssembleyGenerator::generate_CallsToConstructAllNameSpace(std::ostream& /*output*/)
{
}

void AssembleyGenerator::generate_CallToEntryPoint(std::string const& /*entryPoint*/, std::ostream& /*output*/)
{
}

void AssembleyGenerator::generate_AllNSConstructors(std::ostream& /*output*/)
{
}
