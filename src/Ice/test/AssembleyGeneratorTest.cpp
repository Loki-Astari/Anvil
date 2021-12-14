#include <gtest/gtest.h>

#include "AssembleyGenerator.h"
#include "Declaration.h"
#include "Semantic.h"
#include "test/Utility.h"

using namespace ThorsAnvil::Anvil::Ice;

struct AssembleyGeneratorCompiler
{
    NamespaceDecOrder   namespaceDecOrder;
    Namespace           globalScope;
    std::stringstream   error;
    AssembleyGenerator  generator;

    AssembleyGeneratorCompiler()
        : globalScope("")
        , generator(namespaceDecOrder, globalScope, error)
    {}

    bool compile(std::istream& input)
    {
        Lexer           lexer(input, error);
        Storage         storage;
        Semantic        semanticAnalyser(lexer, namespaceDecOrder, globalScope, storage, error);
        Parser          parser(lexer, semanticAnalyser);

        return parser.parse();
    }

    void generate(std::ostream& output)
    {
        generator.generate("", output);
    }
};

bool getNextCommandLine(std::istream& stream, std::string& line)
{
    // Find a line with meaning.
    // i.e. ignore lines that are comments and load the 
    // first line that is not a  comment.
    while (std::getline(stream, line))
    {
        if (line.size() > 2 && line[0] == '/' && line[1] == '/')
        {
            continue;
        }
        break;
    }
    return static_cast<bool>(stream);
}

TEST(AssembleyGeneratorTest, Std)
{
    AssembleyGeneratorCompiler      compiler;
    std::istringstream              input(R"(

namespace Std
{
}
    )");

    ASSERT_TRUE(compiler.compile(input));
    EXPECT_EQ(compiler.namespaceDecOrder.size(), 1);

    std::stringstream   output;
    compiler.generate(output);

    std::stringstream fullOutput(std::move(output.str()));
    std::string       line;

    bool readOK = getNextCommandLine(fullOutput, line);
    ASSERT_TRUE(readOK);
    EXPECT_EQ(line, "CMD Init 0 10000");
}

TEST(AssembleyGeneratorTest, Sys)
{
    AssembleyGeneratorCompiler      compiler;
    std::istringstream              input(R"(

namespace Std
{
    class Console {}
    console: Console;
}
    )");

    ASSERT_TRUE(compiler.compile(input));
    EXPECT_EQ(compiler.namespaceDecOrder.size(), 1);

    std::stringstream   output;
    compiler.generate(output);

    std::stringstream fullOutput(std::move(output.str()));
    std::string       line;

    bool readOK = getNextCommandLine(fullOutput, line);
    ASSERT_TRUE(readOK);
    EXPECT_EQ(line, "CMD Init 1 10000");
}
