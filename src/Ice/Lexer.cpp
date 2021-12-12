#include "Lexer.h"

using namespace ThorsAnvil::Anvil::Ice;

Lexer::Lexer(std::istream& input, std::ostream& output)
    : yyFlexLexer(&input, &output)
{}

std::string_view Lexer::lexem() const
{
    std::string_view    tokenView(YYText(), YYLeng());
    while (tokenView.size() > 0 && tokenView[tokenView.size() - 1] == '\0')
    {
        tokenView.remove_suffix(1);
    }
    return tokenView;
}
