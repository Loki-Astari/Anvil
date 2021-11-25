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
    protected:
    Lexer&          lexer;
    private:
    std::ostream&   output;
    std::string     currentLine;
    std::size_t     lineNo;
    std::size_t     offset;
    public:
        Action(Lexer& lexer, std::ostream& output = std::cerr);
        virtual ~Action();

        // Messaging
        void log(char const* msg);
        [[ noreturn ]] void error(std::string const& msg);

        // Lexing
        virtual void invalidCharacter()                               {addToLine();error("Invalid Character");}
        virtual void ignoreSpace()                                    {addToLine();}
        virtual void newLine()                                        {resetLine();}
        virtual void token();


        // Parsing
        virtual void assertNoStorage(Int)                             {}
        virtual void releaseStorage(Int)                              {}
        virtual Int  generateAnonName()                               {return 0;}

        virtual Int identifierCreate(std::string_view /*view*/)       {return 0;}
        virtual Int identifierCheckObject(Int /*id*/)                 {return 0;}
        virtual Int identifierCheckType(Int /*id*/)                   {return 0;}
        virtual Int identifierCheckNS(Int /*id*/)                     {return 0;}

        virtual Int fullIdentiferCreate()                             {return 0;}
        virtual Int fullIdentiferAddPath(Int /*fp*/, Int /*id*/)      {return 0;}

        virtual Int paramListCreate()                                 {return 0;}
        virtual Int paramListAdd(Int /*pl*/, Int /*type*/)            {return 0;}

        virtual Int objectListCreate()                                {return 0;}
        virtual Int objectListAdd(Int /*ol*/, Int /*object*/)         {return 0;}

        virtual Int findTypeInScope(Int /*fp*/)                       {return 0;}
        virtual Int findObjectInScope(Int /*fp*/)                     {return 0;}

        virtual Int scopeAddNamespace(Int /*name*/)                   {return 0;}
        virtual Int scopeAddClass(Int /*name*/)                       {return 0;}
        virtual Int scopeAddArray(Int /*name*/, Int /*type*/)         {return 0;}
        virtual Int scopeAddMap(Int /*name*/, Int /*k*/, Int /*v*/)   {return 0;}
        virtual Int scopeAddFunc(Int /*name*/, Int /*p*/, Int /*r*/)  {return 0;}
        virtual Int scopeAddObject(Int /*name*/, Int /*t*/, Int /*i*/){return 0;}
        virtual Int scopeAddStatement(Int /*s*/)                      {return 0;}
        virtual Int scopeAddCodeBlock()                               {return 0;}
        virtual Int scopeClose(Int /*oldSCope*/)                      {return 0;}

        virtual Int addLiteralString()                                {return 0;}

        virtual Int codeAddFunctionCall(Int /*obj*/, Int /*param*/)   {return 0;}
    private:
        void addToLine();
        void resetLine();

};

}

#endif
