#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Action.h"
#include "ice.tab.hpp"

#include <fstream>

TEST(ParserTest, ReadIceFile)
{
    std::ifstream   file("test/data/IceFile.ice");

    using ThorsAnvil::Anvil::Ice::CompileAction;
    using ThorsAnvil::Anvil::Ice::Parser;

    CompileAction   action;
    Parser          parser(action, file);

    EXPECT_TRUE(parser.parse());
}
