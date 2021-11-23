#include "Lexer.h"

using namespace ThorsAnvil::Anvil::Ice;

Lexer::Lexer(std::istream& input, std::ostream& output)
    : yyFlexLexer(&input, &output)
{}

std::string_view Lexer::lexem() const
{
    return std::string_view(YYText(), YYLeng());
}
