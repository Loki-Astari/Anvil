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
    std::string     currentLine;
    std::size_t     lineNo;
    std::size_t     offset;
    public:
        Action(std::ostream& output = std::cerr);
        virtual ~Action();

        // Messaging
        void log(char const* msg);
        [[ noreturn ]] void error(Lexer& lexer, std::string const& msg);

        // Lexing
        virtual void invalidCharacter(Lexer& lexer)                                 {addToLine(lexer);error(lexer, "Invalid Character");}
        virtual void ignoreSpace(Lexer& lexer)                                      {addToLine(lexer);}
        virtual void newLine(Lexer& lexer)                                          {resetLine(lexer);}
        virtual void token(Lexer& lexer);


        // Parsing
        virtual Int identifierCreate(Lexer& /*lexer*/)                              {return 0;}
        virtual Int identifierCheckObject(Lexer& /*lexer*/, Int /*id*/)             {return 0;}
        virtual Int identifierCheckType(Lexer& /*lexer*/, Int /*id*/)               {return 0;}
        virtual Int identifierCheckNS(Lexer& /*lexer*/, Int /*id*/)                 {return 0;}

        virtual Int fullIdentiferCreate(Lexer& /*lexer*/)                           {return 0;}
        virtual Int fullIdentiferAddPath(Lexer& /*lexer*/, Int /*fp*/, Int /*id*/)  {return 0;}
        virtual Int findTypeInScope(Lexer& /*lexer*/, Int /*fp*/)                   {return 0;}
    private:
        void addToLine(Lexer& lexer);
        void resetLine(Lexer& lexer);

};

}

#endif
