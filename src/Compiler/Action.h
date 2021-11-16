#ifndef THORSANVIL_ANVIL_ICE_ACTION_H
#define THORSANVIL_ANVIL_ICE_ACTION_H

#include <cstdint>


namespace ThorsAnvil::Anvil::Ice
{

using Int = std::intptr_t;

class Lexer;

class Action
{
    public:
        Action();
        virtual ~Action()   = 0;
};

}

#endif
