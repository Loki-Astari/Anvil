#include "Lexer.h"

using namespace ThorsAnvil::Anvil::Ice;

Lexer::Lexer(std::istream& stream, Action& action)
    : yyFlexLexer(&stream, &std::cerr)
    , action(action)
{}

std::string_view Lexer::lexem() const
{
    return std::string_view(YYText(), YYLeng());
}
