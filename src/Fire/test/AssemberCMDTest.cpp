#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(AssemblerCMD, EmptyInputIsOK)
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

TEST(AssemblerCMD, IgnoreComments)
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

TEST(AssemblerCMD, UknownComand)
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

TEST(AssemblerCMD, CMD_NoOp)
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

TEST(AssemblerCMD, CMD_Kill)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Kill  // No Value is OK
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 1, result);
    if (memory.size() == 1) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFC00, Assembler::Act_CMD | Assembler::Cmd_Kill, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerCMD, CMD_KillOverSizeValue)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Kill 1024  // Max value 1023
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerCMD, CMD_Init)
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
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFC00, Assembler::Act_CMD | Assembler::Cmd_Init, result);
        EXPECT_EQ_OR_LOG(test, memory[0] & 0x03FF, 1023, result);
        EXPECT_EQ_OR_LOG(test, memory[1], 65535, result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerCMD, CMD_InitBadGlobalValue)
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

TEST(AssemblerCMD, CMD_InitBadStorageValue)
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

TEST(AssemblerCMD, CMD_ImportFire)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Import BestFileX.fire
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 6, result);
    if (memory.size() == 6) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFF00, Assembler::Act_CMD | Assembler::Cmd_Import | Assembler::Cmd_ImportFileFire, result);
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

TEST(AssemblerCMD, CMD_ImportDLL)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Import DllFile
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 5, result);
    if (memory.size() == 5) {
        EXPECT_EQ_OR_LOG(test, memory[0] & 0xFF00, Assembler::Act_CMD | Assembler::Cmd_Import | Assembler::Cmd_ImportFileDLL, result);
        EXPECT_EQ_OR_LOG(test, memory[0] & 0x00FF, std::string("DllFile").size(), result);
        EXPECT_EQ_OR_LOG(test, memory[1], ((static_cast<Instruction>('D') << 8) | static_cast<Instruction>('l')), result);
        EXPECT_EQ_OR_LOG(test, memory[2], ((static_cast<Instruction>('l') << 8) | static_cast<Instruction>('F')), result);
        EXPECT_EQ_OR_LOG(test, memory[3], ((static_cast<Instruction>('i') << 8) | static_cast<Instruction>('l')), result);
        EXPECT_EQ_OR_LOG(test, memory[4], ((static_cast<Instruction>('e') << 8)), result);
    }
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerCMD, CMD_ImportNoFileName)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Import
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}


TEST(AssemblerCMD, CMD_ImportExcessiveLargeFile)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Import 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 129, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), true, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

TEST(AssemblerCMD, CMD_ImportExcessiveLargeFileOverLimit)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD Import 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
)"));

    Assembler                   assembler(file, result, memory);

    bool test = false;
    EXPECT_EQ_OR_LOG(test, memory.size(), 0, result);
    EXPECT_EQ_OR_LOG(test, assembler.isOK(), false, result);
    EXPECT_TRUE_OR_DEBUG(!test, result);
}

