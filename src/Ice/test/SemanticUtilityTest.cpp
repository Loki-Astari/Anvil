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

TEST(SemanticUtilityTest, CallGenerateNameMultipleTimesMakeSureTheyIncrease)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);

    std::vector<std::string>    values;
    for (int loop = 0; loop < 1000; ++loop)
    {
        ThorsAnvil::Anvil::Ice::Int index = compiler.semanticAnalyser.generateAnonName();
        ThorsAnvil::Anvil::Ice::SAccessString access(compiler.storage, index);
        values.emplace_back(access.get());
    }

    for (int loop = 0; loop < values.size(); ++loop)
    {
        for (int check = loop + 1; check < values.size(); ++check)
        {
            EXPECT_NE(values[loop], values[check]);
        }
    }
}

TEST(SemanticUtilityTest, CheckStorageCheckWorksValid)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);

    EXPECT_NO_THROW(
        compiler.semanticAnalyser.assertNoStorage(0)
    );
}

TEST(SemanticUtilityTest, CheckStorageCheckWorksInvalid)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);

    EXPECT_THROW(
        compiler.semanticAnalyser.assertNoStorage(1),
        std::runtime_error
    );
}

TEST(SemanticUtilityTest, CheckInvalidNamespaceName)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);
    ThorsAnvil::Anvil::Ice::Int name = compiler.semanticAnalyser.identifierCreate("badNamespaceName");

    EXPECT_THROW(
        compiler.semanticAnalyser.identifierCheckNS(name),
        std::runtime_error
    );
}

TEST(SemanticUtilityTest, CheckInvalidTypeName)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);
    ThorsAnvil::Anvil::Ice::Int name = compiler.semanticAnalyser.identifierCreate("badTypeName");

    EXPECT_THROW(
        compiler.semanticAnalyser.identifierCheckType(name),
        std::runtime_error
    );
}

TEST(SemanticUtilityTest, CheckInvalidObjectName)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);
    ThorsAnvil::Anvil::Ice::Int name = compiler.semanticAnalyser.identifierCreate("BadObjectName");

    EXPECT_THROW(
        compiler.semanticAnalyser.identifierCheckObject(name),
        std::runtime_error
    );
}

TEST(SemanticUtilityTest, TestBadScopeClosure)
{
    std::stringstream           result;
    std::stringstream           input;
    SemanticCompiler            compiler(input, result);
    ThorsAnvil::Anvil::Ice::Int name1 = compiler.semanticAnalyser.identifierCreate("First_Scope_Name");
    ThorsAnvil::Anvil::Ice::Int ns1   = compiler.semanticAnalyser.scopeAddNamespace(name1);
    ThorsAnvil::Anvil::Ice::Int name2 = compiler.semanticAnalyser.identifierCreate("Second_Scope_Name");
    ThorsAnvil::Anvil::Ice::Int ns2   = compiler.semanticAnalyser.scopeAddNamespace(name2);

    EXPECT_THROW(
        compiler.semanticAnalyser.scopeClose(ns1),
        std::runtime_error
    );
}

