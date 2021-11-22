#ifndef THORSANVIL_ANVIL_ICE_LEXER_H
#define THORSANVIL_ANVIL_ICE_LEXER_H

#ifndef IN_LEXER
#include <FlexLexer.h>
#endif

#include "Action.h"
#include <string_view>

namespace ThorsAnvil::Anvil::Ice
{

class Lexer: public yyFlexLexer
{
    Action&     action;
    public:
        Lexer(Action& action, std::istream& input = std::cin, std::ostream& output = std::cout);
        std::string_view lexem() const;
        virtual int yylex() override;
};

}

#endif
