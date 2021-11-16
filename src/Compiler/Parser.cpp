#include "Parser.h"

using namespace ThorsAnvil::Anvil::Ice;

Parser::Parser(std::istream& stream, Action& action)
    : lexer(stream, action)
    , parser(lexer, action)
{}

bool Parser::parse()
{
    return parser.parse() == 0;
}
