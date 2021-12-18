#include "AssembleyGenerator.h"

#include "Lexer.h"
#include "Semantic.h"
#include "Parser.h"
#include "Declaration/Inline.h"

#include <numeric>

using namespace ThorsAnvil::Anvil::Ice;

AssembleyGenerator::AssembleyGenerator(NamespaceDecOrder& namespaceDecOrder, Scope& scope, std::ostream& error)
    : namespaceDecOrder(namespaceDecOrder)
    , globalScope(scope)
    , error(error)
{}

void AssembleyGenerator::generate(std::string const& entryPoint, std::ostream& output)
{
    generate_InitStatement(output);
    generate_CallsToConstructAllNameSpace(output);
    generate_CallToEntryPoint(entryPoint, output);
    generate_CallsToDestructAllNameSpace(output);
    generate_EndMainSection(output);
    generate_AllNSCode(output);
}

void AssembleyGenerator::generate_InitStatement(std::ostream& output)
{
    std::size_t globalSize = std::accumulate(std::begin(namespaceDecOrder), std::end(namespaceDecOrder), std::size_t{0},
                                                [](std::size_t lhs, auto const& rhs)
                                                {
                                                    return lhs + rhs.get().size();
                                                }
                                            );
    output <<   "// Initialize the Global Size\n"
           <<   "CMD Init " << globalSize << " " << 10000 << "\n"
           <<   "\n";
}

void AssembleyGenerator::generate_CallsToConstructAllNameSpace(std::ostream& output)
{
    std::size_t lastNSSize = 0;

    output  << "// Phase 1: Construct all namespace objects.\n"
            << "// Set up Expr-1 to set up the global namespace\n"
            << "// Each constructor will have 'This' register point at the first object in the namespace.\n"
            << "ADDR LRR Expr-1 = Global + 0\n"
            << "\n";

    for (auto const& ns: make_View<Forward>(namespaceDecOrder))
    {
        output  <<  "// Initialize all members of " << ns.get().fullPathName() << "\n"
                <<  "ADDR INC Expr-1 " << lastNSSize << "\n"
                <<  "JUMP Call Expr-1 " << ns.get().fullPathName() << "-$constructor\n"
                <<  "\n";
        lastNSSize += ns.get().size();
    }
}

void AssembleyGenerator::generate_CallToEntryPoint(std::string const& /*entryPoint*/, std::ostream& output)
{
    output  << "// Pase 2: Call the entry function\n"
            << "// TODO: Call main function here\n"
            << "\n";
}

void AssembleyGenerator::generate_CallsToDestructAllNameSpace(std::ostream& output)
{
    std::size_t globalSize = std::accumulate(std::begin(namespaceDecOrder), std::end(namespaceDecOrder), std::size_t{0},
                                                [](std::size_t lhs, auto const& rhs)
                                                {
                                                    return lhs + rhs.get().size();
                                                }
                                            );

    output  << "// Phase 3: Destruct all namespace objects.\n"
            << "// Set up Expr-1 to set up the global namespace\n"
            << "// Each destructor will have 'This' register point at the first object in the namespace.\n"
            << "ADDR LRR Expr-1 = Global + " << globalSize << "\n"
            << "\n";

    for (auto const& ns: make_View<Reverse>(namespaceDecOrder))
    {
        output  <<  "// Initialize all members of " << ns.get().fullPathName() << "\n"
                <<  "ADDR DEC Expr-1 " << ns.get().size() << "\n"
                <<  "JUMP Call Expr-1 " << ns.get().fullPathName() << "-$destructor\n"
                <<  "\n";
    }
}

void AssembleyGenerator::generate_EndMainSection(std::ostream& output)
{
    output  << "// Phase 4: Exit application\n"
            << "CMD Kill 0\n"
            << "\n";
}

void AssembleyGenerator::generate_AllNSCode(std::ostream& output)
{
    for (auto const& nsref: make_View<Forward>(namespaceDecOrder))
    {
        Namespace&  ns = nsref.get();
        generate_SpecificNSCodeBlock(output, ns, "$constructor");
        generate_SpecificNSCodeBlock(output, ns, "$destructor");
    }
}

void AssembleyGenerator::generate_SpecificNSCodeBlock(std::ostream& output, Namespace& ns, std::string const& name)
{
    auto find = ns.get(name + "$Code");
    if (!find.first)
    {
        throw std::runtime_error("Expected there to be a " + name + " in a Namespace");
    }

    Decl&       codeDecl    = *find.second->second;
    CodeBlock&  codeBlock   = dynamic_cast<CodeBlock&>(codeDecl);

    output << ns.fullPathName() << "-" << name << ":\n";
    codeBlock.generateAssembley(output);
    output << "JUMP Ret Al\n";
}
