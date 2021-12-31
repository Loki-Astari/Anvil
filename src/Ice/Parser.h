#ifndef THORSANVIL_ANVIL_ICE_PARSER_H
#define THORSANVIL_ANVIL_ICE_PARSER_H

#include "Lexer.h"
#include "ice.tab.hpp"

namespace ThorsAnvil::Anvil::Ice
{

class Parser
{
    public:
        Parser(Lexer& lexer, Action& action);
        bool parse();
    private:
        Lexer&          lexer;
        ::yy::Parser    parser;
};

}

#endif
