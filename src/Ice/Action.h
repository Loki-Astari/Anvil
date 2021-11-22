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
        virtual void invalidCharacter(Lexer& lexer)                                     {addToLine(lexer);error(lexer, "Invalid Character");}
        virtual void ignoreSpace(Lexer& lexer)                                          {addToLine(lexer);}
        virtual void newLine(Lexer& lexer)                                              {resetLine(lexer);}
        virtual void token(Lexer& lexer);


        // Parsing
        virtual void assertNoStorage(Lexer&, Int)                                       {}
        virtual void releaseStorage(Lexer&, Int)                                        {}
        virtual Int  generateAnonName(Lexer&)                                           {return 0;}

        virtual Int identifierCreate(Lexer& /*lexer*/)                                  {return 0;}
        virtual Int identifierCheckObject(Lexer& /*lexer*/, Int /*id*/)                 {return 0;}
        virtual Int identifierCheckType(Lexer& /*lexer*/, Int /*id*/)                   {return 0;}
        virtual Int identifierCheckNS(Lexer& /*lexer*/, Int /*id*/)                     {return 0;}

        virtual Int fullIdentiferCreate(Lexer& /*lexer*/)                               {return 0;}
        virtual Int fullIdentiferAddPath(Lexer& /*lexer*/, Int /*fp*/, Int /*id*/)      {return 0;}

        virtual Int paramListCreate(Lexer& /*lexer*/)                                   {return 0;}
        virtual Int paramListAdd(Lexer& /*lexer*/, Int /*pl*/, Int /*type*/)            {return 0;}

        virtual Int findTypeInScope(Lexer& /*lexer*/, Int /*fp*/)                       {return 0;}

        virtual Int scopeAddNamespace(Lexer& /*lexer*/, Int /*name*/)                   {return 0;}
        virtual Int scopeAddClass(Lexer& /*lexer*/, Int /*name*/)                       {return 0;}
        virtual Int scopeAddArray(Lexer& /*lexer*/, Int /*name*/, Int /*type*/)         {return 0;}
        virtual Int scopeAddMap(Lexer& /*lexer*/, Int /*name*/, Int /*k*/, Int /*v*/)   {return 0;}
        virtual Int scopeAddFunc(Lexer& /*lexer*/, Int /*name*/, Int /*p*/, Int /*r*/)  {return 0;}
        virtual Int scopeAddObject(Lexer& /*lexer*/, Int /*name*/, Int)                 {return 0;}
        virtual Int scopeAddStatement(Lexer& /*lexer*/, Int /*s*/)                      {return 0;}
        virtual Int scopeClose(Lexer& /*lexer*/, Int /*oldSCope*/)                      {return 0;}

    private:
        void addToLine(Lexer& lexer);
        void resetLine(Lexer& lexer);

};

}

#endif
