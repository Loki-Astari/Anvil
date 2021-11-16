#ifndef THORSANVIL_ANVIL_ICE_LEXER_H
#define THORSANVIL_ANVIL_ICE_LEXER_H

#ifndef IN_LEXER
#include <FlexLexer.h>
#endif

#include "Action.h"

namespace ThorsAnvil::Anvil::Ice
{

class Lexer: public yyFlexLexer
{
    Action&     action;
    public:
        Lexer(std::istream& stream, Action& action);
        virtual int yylex();
};

}

#endif
