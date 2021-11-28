#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler, EmptyInputIsOK)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, IgnoreComments)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
    // This Assembly contains
    // only comments that should be ignored and generate no code.
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, UknownComand)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD BadCommand Generate error
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_NoOp)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD NoOp
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() > 0) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xF000, Assembler::Act_CMD | Assembler::Cmd_NoOp, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_Kill)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Kill
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xF000, Assembler::Act_CMD | Assembler::Cmd_Kill, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_Init)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Init 1023 65535
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 2, result);
    if (memory.size() == 2) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xF000, Assembler::Act_CMD | Assembler::Cmd_Init, result);
        EXPECT_EQ_OR_LOG(test, memory[0] & 0x0FFF, 1023, result);
        EXPECT_EQ_OR_LOG(test, memory[1], 65535, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_InitBadGlobalValue)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Init 1024 65535
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_InitBadStorageValue)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Init 1023 65536
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_LoadFire)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Load BestFileX.fire
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 6, result);
    if (memory.size() == 6) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFF00, Assembler::Act_CMD | Assembler::Cmd_Load | Assembler::LoadFileFire, result);
        EXPECT_EQ_OR_LOG(test, memory[0] & 0x00FF, std::string("BestFileX").size(), result);
        EXPECT_EQ_OR_LOG(test, memory[1], ((static_cast<Instruction>('B') << 8) | static_cast<Instruction>('e')), result);
        EXPECT_EQ_OR_LOG(test, memory[2], ((static_cast<Instruction>('s') << 8) | static_cast<Instruction>('t')), result);
        EXPECT_EQ_OR_LOG(test, memory[3], ((static_cast<Instruction>('F') << 8) | static_cast<Instruction>('i')), result);
        EXPECT_EQ_OR_LOG(test, memory[4], ((static_cast<Instruction>('l') << 8) | static_cast<Instruction>('e')), result);
        EXPECT_EQ_OR_LOG(test, memory[5], ((static_cast<Instruction>('X') << 8)), result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_LoadDLL)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Load DllFile
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 5, result);
    if (memory.size() == 5) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFF00, Assembler::Act_CMD | Assembler::Cmd_Load | Assembler::LoadFileDLL, result);
        EXPECT_EQ_OR_LOG(test, memory[0] & 0x00FF, std::string("DllFile").size(), result);
        EXPECT_EQ_OR_LOG(test, memory[1], ((static_cast<Instruction>('D') << 8) | static_cast<Instruction>('l')), result);
        EXPECT_EQ_OR_LOG(test, memory[2], ((static_cast<Instruction>('l') << 8) | static_cast<Instruction>('F')), result);
        EXPECT_EQ_OR_LOG(test, memory[3], ((static_cast<Instruction>('i') << 8) | static_cast<Instruction>('l')), result);
        EXPECT_EQ_OR_LOG(test, memory[4], ((static_cast<Instruction>('e') << 8)), result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_LoadNoFileName)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Load
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


TEST(Assembler, CMD_LoadExcessiveLargeFile)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Load 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 129, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(Assembler, CMD_LoadExcessiveLargeFileOverLimit)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Load 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

