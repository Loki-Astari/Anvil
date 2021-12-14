#include "AssembleyGenerator.h"

#include "Lexer.h"
#include "Semantic.h"
#include "Parser.h"

#include <numeric>

using namespace ThorsAnvil::Anvil::Ice;

namespace
{
    enum ViewType {Forward, Reverse};

    template<ViewType V, typename I>
    struct View
    {
        I beginRange;
        I endRange;
        using ForwardI = I;
        using ReverseI = std::reverse_iterator<I>;
        auto begin() {if constexpr (V == Forward) {return beginRange;} else {return ReverseI(endRange);}}
        auto end()   {if constexpr (V == Forward) {return endRange;} else {return ReverseI(beginRange);}}
    };

    // Helper function to make view without need to know type.
    template<ViewType V, typename I>
    View<V, I> make_View(I begin, I end) {return View<V, I>{begin, end};}

    // Helper function to make view without need to know type.
    template<ViewType V, typename C>
    View<V, typename C::iterator> make_View(C& c) {return View<V, typename C::iterator>{std::begin(c), std::end(c)};}
}

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
    generate_AllNSConstructors(output);
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

    output  << "// Set up Expr-1 to set up the global namespace\n"
            << "ADDR LRR Expr-1 = Global + 0\n"
            << "\n";

    for (auto const& ns: make_View<Forward>(namespaceDecOrder))
    {
        output  <<  "ADDR INC Expr-1 " << lastNSSize << "\n"
                <<  "JUMP Call Expr-1 " << ns.get().fullPathName() << "::$constructor\n"
                <<  "\n";
        lastNSSize += ns.get().size();
    }
}

void AssembleyGenerator::generate_CallToEntryPoint(std::string const& /*entryPoint*/, std::ostream& output)
{
    output  << "\n"
            << "// Call main function here\n"
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

    output  << "// Set up Expr-1 to set up the global namespace\n"
            << "ADDR LRR Expr-1 = Global + " << globalSize << "\n"
            << "\n";

    for (auto const& ns: make_View<Reverse>(namespaceDecOrder))
    {
        output  <<  "ADDR DEC Expr-1 " << ns.get().size() << "\n"
                <<  "JUMP Call Expr-1 " << ns.get().fullPathName() << "::$destructor\n"
                <<  "\n";
    }
}

void AssembleyGenerator::generate_EndMainSection(std::ostream& output)
{
    output  << "// Exit application\n"
            << "CMD Kill 0\n"
            << "\n";
}

void AssembleyGenerator::generate_AllNSConstructors(std::ostream& /*output*/)
{
}
