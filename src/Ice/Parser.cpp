#include "Parser.h"

using namespace ThorsAnvil::Anvil::Ice;

Parser::Parser(Lexer& lexer, Action& action)
    : lexer(lexer)
    , parser(lexer, action)
{}

bool Parser::parse()
{
    return parser.parse() == 0;
}
