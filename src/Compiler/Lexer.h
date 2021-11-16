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
        Lexer(std::istream& stream, Action& action);
        std::string_view lexem() const;
        virtual int yylex() override;
};

}

#endif
