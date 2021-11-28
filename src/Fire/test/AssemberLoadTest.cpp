#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler, LoadGlobalLiteralMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global Literal 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegGlobal[0] | Assembler::FromLiteral | Assembler::ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadStackLiteralMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack Literal 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegStack[0] | Assembler::FromLiteral | Assembler::ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadGlobalLiteralOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global Literal 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadThisReadMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This Read -31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegThis[0] | Assembler::FromRead | Assembler::MarkNeg | Assembler::ValueSmall | 31, result);
        // 0001 1001 0001 1111
        // 0001 1001 0101 1111
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadExprReadMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr Read -2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::FromRead | Assembler::MarkNeg | Assembler::ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadGlobalReadOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global Read -2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

///-----

TEST(Assembler, LoadGlobalRegMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global Reg Stack 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegGlobal[0] | Assembler::FromReg | Assembler::RegStack[1] | Assembler::ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadStackRegMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack Reg This 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegStack[0] | Assembler::FromReg | Assembler::RegThis[1] | Assembler::ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadGlobalRegOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global Reg This 2097152
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadThisIndReadMax1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load This IndRead Expr 31
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegThis[0] | Assembler::FromIndRead | Assembler::RegExpr[1] | Assembler::ValueSmall | 31, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadExprIndReadMax2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Expr IndRead Global 2097151
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0], Assembler::Act_Load | Assembler::RegExpr[0] | Assembler::FromIndRead | Assembler::RegGlobal[1] | Assembler::ValueLarge | (2097151 >> 16) , result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadGlobalIndReadOverSized)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Global IndRead Stack -1
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

///-----

TEST(Assembler, LoadInvalidDstReg)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Invalid IndRead Stack 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadInvalidFrom)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack Invalid Stack 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadInvalidSrcReg)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack IndRead Invalid 10
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, LoadInvalidOffset)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        Load Stack IndRead Expr XX
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


