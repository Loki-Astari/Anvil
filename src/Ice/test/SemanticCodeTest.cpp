#include <gtest/gtest.h>

#include "Semantic.h"
#include "test/Utility.h"

struct SemanticCompiler
{
    ThorsAnvil::Anvil::Ice::StandardScope   globalScope;
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Semantic        semanticAnalyser;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    SemanticCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , semanticAnalyser(lexer, globalScope, output)
        , parser(lexer, semanticAnalyser)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(SemanticCodeTest, CodeMustBeInFunctionInvalid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    Sys::console.print("Test");
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Adding code but not inside a code block", result);
}

TEST(SemanticCodeTest, CodeMustBeInFunctionValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    main : func { -> Std::Integer} {
        Sys::console.print("Test");
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

