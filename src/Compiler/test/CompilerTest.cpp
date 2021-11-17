#include <gtest/gtest.h>

#include "Compiler.h"

TEST(CompilerTest,NameStatExprFuncCall)
{
    using ThorsAnvil::Anvil::Ice::Compiler;

    Compiler          compiler("test/data/NameStatExprFuncCall.ice");
    compiler.go();
}

/*
TEST(CompilerTest,NameTypeArray)
{
    std::ifstream   file("test/data/NameTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameTypeFunc)
{
    std::ifstream   file("test/data/NameTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameTypeMap)
{
    std::ifstream   file("test/data/NameTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameTypeObject)
{
    std::ifstream   file("test/data/NameTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameVariableAnonTypeArray)
{
    std::ifstream   file("test/data/NameVariableAnonTypeArray.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameVariableAnonTypeFunc)
{
    std::ifstream   file("test/data/NameVariableAnonTypeFunc.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameVariableAnonTypeMap)
{
    std::ifstream   file("test/data/NameVariableAnonTypeMap.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameVariableAnonTypeObject)
{
    std::ifstream   file("test/data/NameVariableAnonTypeObject.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}

TEST(CompilerTest,NameVariableDecl)
{
    std::ifstream   file("test/data/NameVariableDecl.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Compiler;

    CompileAction   action;
    Compiler          compiler(action, file);

    EXPECT_TRUE(compiler.parse());
}
*/

