#include "Generator.h"
#include "Declaration.h"

#include <ostream>
#include <numeric>

using namespace ThorsAnvil::Anvil::Ice;

Generator::Generator(Scope& globalScope, NamespaceDecOrder& namespaceDecOrder)
    : globalScope(globalScope)
    , namespaceDecOrder(namespaceDecOrder)
{}

void Generator::generate(std::ostream& output)
{
    generateInitialize(output);
    generateNameSpaceInit(output);
    generateCallToApplication(output);
    generateNameSpaceDeInit(output);
    generateCallToQuitApp(output);

    for (auto const& ns: make_View<Forward>(namespaceDecOrder))
    {
        ns.get().generateCode(*this, output);
    }
}

void Generator::generateInitialize(std::ostream& output)
{
    std::size_t size = std::accumulate(std::begin(namespaceDecOrder), std::end(namespaceDecOrder), 0,
                                        [](std::size_t current, NamespaceRef const& ns)
                                        {
                                            return current + ns.get().getMemberSize();
                                        });
    output << "// Set up the application storage space\n"
           << "CMD Init " << size << " 10,000\n"
           << "\n";
}

void Generator::generateCallToApplication(std::ostream& output)
{
    output << "// Call entry point\n"
           << "ADDR LRR Expr-1 = StackPointer + 0\n"
           << "JUMP Call Expr-1 Name_Space-main{Void}\n"
           << "\n";
}

void Generator::generateCallToQuitApp(std::ostream& output)
{
    output << "// Exit Application\n"
           << "CMD Kill\n"
           << "\n";
}

void Generator::generateNameSpaceInit(std::ostream& output)
{
    std::size_t offset = 0;
    for (auto const& ns: make_View<Forward>(namespaceDecOrder))
    {
        output << "// Initialize: " << ns.get().declName(true) << "\n"
               << "ADDR LRR Expr-1 = Global + " << offset << "\n"
               << "JUMP Call Expr-1 " << ns.get().declName(true) << "-$constructor{}\n"
               << "\n";

        offset += ns.get().getMemberSize();
    }
}
void Generator::generateNameSpaceDeInit(std::ostream& output)
{
    std::size_t offset = std::accumulate(std::begin(namespaceDecOrder), std::end(namespaceDecOrder), 0,
                                        [](std::size_t current, NamespaceRef const& ns)
                                        {
                                            return current + ns.get().getMemberSize();
                                        });
    for (auto const& ns: make_View<Reverse>(namespaceDecOrder))
    {
        offset -= ns.get().getMemberSize();

        output << "// Deinitialize: " << ns.get().declName(true) << "\n"
               << "ADDR LRR Expr-1 = Global + " << offset << "\n"
               << "JUMP Call Expr-1 " << ns.get().declName(true) << "-$destructor{}\n"
               << "\n";
    }
}

void Generator::outputNotImplementedYet(Decl const& decl, std::ostream& output)
{
    output << "// Code Generation Not Implemented: " << decl.declName() << "\n\n";
}

