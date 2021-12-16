#include <gtest/gtest.h>


#include "Semantic.h"
#include "Declaration/StandardScope.h"
#include "test/Utility.h"
#include "test/SemanticTest.h"

struct SemanticCompiler
{
    ThorsAnvil::Anvil::Ice::NamespaceDecOrder   namespaceDecOrder;
    ThorsAnvil::Anvil::Ice::StandardScope       globalScope;
    ThorsAnvil::Anvil::Ice::Lexer               lexer;
    ThorsAnvil::Anvil::Ice::Storage             storage;
    ThorsAnvil::Anvil::Ice::Semantic            semanticAnalyser;
    ThorsAnvil::Anvil::Ice::Parser              parser;

    SemanticCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , semanticAnalyser(lexer, namespaceDecOrder, globalScope, storage, output)
        , parser(lexer, semanticAnalyser)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(SemanticCodeTest, NestedFuncitonWorking)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    namespace X
    {
        namespace Y
        {
            test: func { Std::String -> Void } {}
        }
    }

    main: func { Std::String -> Void} {
        My_Work::X::Y::test("Test");
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticCodeTest, CodeInsideNamespaceAddedTONamespaceConterutor)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    Sys::console.print("Test");
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticCodeTest, ValidFunctionCallShouldCompile)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    main : func { Std::String -> Void} {
        Sys::console.print("Test");
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticCodeTest, InvalidParamFunctionCallShouldNotCompile)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace My_Work
{
    main : func { Std::String -> Void} {
        Sys::console.print();
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not enough parameters were passed to the function", result);
}

