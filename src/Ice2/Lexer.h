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
    bool started;
    public:
        Lexer(std::istream& input = std::cin, std::ostream& output = std::cerr);
        std::string_view lexem() const;
        virtual int yylex() override {throw std::runtime_error("Wrong Lex Called");}
        virtual int yylexWithAction(Action& action);
        int yylexWithActionGo(Action& action) { started = true; return yylexWithAction(action); }
};

}

#endif
