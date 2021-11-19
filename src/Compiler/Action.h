#ifndef THORSANVIL_ANVIL_ICE_ACTION_H
#define THORSANVIL_ANVIL_ICE_ACTION_H

#include <cstdint>
#include <iostream>


namespace ThorsAnvil::Anvil::Ice
{

using Int = std::intptr_t;

class Lexer;

class Action
{
    std::ostream&   output;
    public:
        Action(std::ostream& output = std::cerr);
        virtual ~Action()   = 0;

        virtual void log(char const* msg);

        virtual Int identifierCreate(Lexer& /*lexer*/)                              {return 0;}
        virtual Int identifierCheckObject(Lexer& /*lexer*/, Int /*id*/)             {return 0;}
        virtual Int identifierCheckType(Lexer& /*lexer*/, Int /*id*/)               {return 0;}
        virtual Int identifierCheckNS(Lexer& /*lexer*/, Int /*id*/)                 {return 0;}

        virtual Int fullIdentiferCreate(Lexer& /*lexer*/)                           {return 0;}
        virtual Int fullIdentiferAddPath(Lexer& /*lexer*/, Int /*fp*/, Int /*id*/)  {return 0;}
        virtual Int findTypeInScope(Lexer& /*lexer*/, Int /*fp*/)                   {return 0;}

};

}

#endif
