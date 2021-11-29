#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(AssemblerLoad, LoadGlobalAbsLiteralMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = Literal 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegGlobal[0] | Assembler::Load_FromLiteral | Assembler::Load_ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackAbsLiteralMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Literal 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegStack[0] | Assembler::Load_FromLiteral | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalAbsLiteralOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = Literal 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisAbsReadMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = Read -31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegThis[0] | Assembler::Load_FromRead | Assembler::Load_MarkNeg | Assembler::Load_ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprAbsReadMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = Read -2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::Load_FromRead | Assembler::Load_MarkNeg | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalAbsReadOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = Read -2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

///-----

TEST(AssemblerLoad, LoadGlobalRelLiteralMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global += Literal 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegGlobal[0] | Assembler::Load_FromLiteral | Assembler::Load_MarkRel | Assembler::Load_ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackRelLiteralMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack += Literal 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegStack[0] | Assembler::Load_FromLiteral | Assembler::Load_MarkRel | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalRelLiteralOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global += Literal 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisRelReadMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This += Read -31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegThis[0] | Assembler::Load_FromRead | Assembler::Load_MarkRel | Assembler::Load_MarkNeg | Assembler::Load_ValueSmall | 31, result);
        // 0001 1001 0001 1111
        // 0001 1001 0101 1111
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprRelReadMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr += Read -2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::Load_FromRead | Assembler::Load_MarkRel | Assembler::Load_MarkNeg | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalRelReadOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global += Read -2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

///-----

TEST(AssemblerLoad, LoadGlobalRegMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = Reg Stack 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegGlobal[0] | Assembler::Load_FromReg | Assembler::RegStack[1] | Assembler::Load_ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadStackRegMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Reg This 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegStack[0] | Assembler::Load_FromReg | Assembler::RegThis[1] | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalRegOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = Reg This 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadThisIndReadMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This = IndRead Expr 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegThis[0] | Assembler::Load_FromIndRead | Assembler::RegExpr[1] | Assembler::Load_ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadExprIndReadMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr = IndRead Global 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::Load_FromIndRead | Assembler::RegGlobal[1] | Assembler::Load_ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadGlobalIndReadOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global = IndRead Stack -1
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

///-----

TEST(AssemblerLoad, LoadInvalidDstReg)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Invalid = IndRead Stack 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadInvalidFrom)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = Invalid Stack 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadInvalidSrcReg)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = IndRead Invalid 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerLoad, LoadInvalidOffset)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack = IndRead Expr XX
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


