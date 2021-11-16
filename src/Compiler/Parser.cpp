#include "Parser.h"

using namespace ThorsAnvil::Anvil::Ice;

Parser::Parser(Action& action, std::istream& input, std::ostream& output)
    : lexer(action, input, output)
    , parser(lexer, action)
{}

bool Parser::parse()
{
    return parser.parse() == 0;
}
