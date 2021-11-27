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
    EXPECT_EQ(memory.size(), 0);
    EXPECT_TRUE(assembler.isOK());
    EXPECT_TRUE_OR_DEBUG(memory.size() == 0 && assembler.isOK(), result);
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
    EXPECT_EQ(memory.size(), 0);
    EXPECT_TRUE(assembler.isOK());
    EXPECT_TRUE_OR_DEBUG(memory.size() == 0 && assembler.isOK(), result);
}

TEST(Assembler, UknownComand)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD BadCommand Generate error
)"));

    Assembler                   assembler(file, result, memory);
    EXPECT_EQ(memory.size(), 0);
    EXPECT_FALSE(assembler.isOK());
    EXPECT_TRUE_OR_DEBUG(memory.size() == 0 && !assembler.isOK(), result);
}

TEST(Assembler, CMD_NOOP)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD NOOP
)"));

    Assembler                   assembler(file, result, memory);
    EXPECT_EQ(memory.size(), 1);
    EXPECT_TRUE(assembler.isOK());
    EXPECT_TRUE_OR_DEBUG(memory.size() == 1 && assembler.isOK(), result);
}

