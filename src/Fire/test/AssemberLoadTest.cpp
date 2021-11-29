#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

// Set Stack
TEST(AssemblerLoad, LoadStackSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Set This
TEST(AssemblerLoad, LoadThisSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Set Expr
TEST(AssemblerLoad, LoadExprSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


// Read Stack
TEST(AssemblerLoad, LoadStackReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Read This
TEST(AssemblerLoad, LoadThisReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Read Expr
TEST(AssemblerLoad, LoadExprReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Global + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * This + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueSmall |
            12,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

// Set Stack
TEST(AssemblerLoad, LoadStackSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Set This
TEST(AssemblerLoad, LoadThisSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Set Expr
TEST(AssemblerLoad, LoadExprSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegGlobal[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegStack[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegThis[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Set | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


// Read Stack
TEST(AssemblerLoad, LoadStackReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegStack[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Read This
TEST(AssemblerLoad, LoadThisReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegThis[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}
// Read Expr
TEST(AssemblerLoad, LoadExprReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Global + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegGlobal[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegStack[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * This + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegThis[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 345, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


// Range Checks

TEST(AssemblerLoad, Check1ByteMaxOK)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueSmall | 31,
            result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, Check1ByteMaxExceeded)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 32
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], 32, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


TEST(AssemblerLoad, Check2ByteMaxOK)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge |
            (2097151 >> 16),
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], (2097151 & 0xFFFF), result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, Check2ByteMaxExceeded)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, Check2ByteMaxNegativeOK)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + -2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0],
            Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::RegExpr[1] | Assembler::Load_Read | Assembler::Load_ValueLarge |
            (-2097151 >> 16) & 0x2F,
            result);
        EXPECT_EQ_OR_LOG(test, memory[1], (-2097151 & 0xFFFF), result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, Check2ByteMaxNegativeExceeded)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr * Expr + -2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

// Bad Value Checks

TEST(AssemblerLoad, BadDst)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Bad * Expr + 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, BadAction)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack $ Expr + 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, BadSrc)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Bad + 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, NotPlus)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Expr @ 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, BadValue)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = This + XX
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


