#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_STANDARD_SCOPE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_STANDARD_SCOPE_H

#include "Namespace.h"
#include "Types.h"
#include "Code.h"

namespace ThorsAnvil::Anvil::Ice
{

using UPVoid        = std::unique_ptr<Void>;
using UPNamespace   = std::unique_ptr<Namespace>;
using UPClass       = std::unique_ptr<Class>;
using UPCodeBlock   = std::unique_ptr<CodeBlock>;
using UPArray       = std::unique_ptr<Array>;
using UPMap         = std::unique_ptr<Map>;
using UPFunc        = std::unique_ptr<Func>;
using UPObject      = std::unique_ptr<Object>;
using UPStatement   = std::unique_ptr<Statement>;

using Scope = Container;

class StandardScope: public Namespace
{
    public:
        StandardScope()
            : Namespace("") // Its the global scope so has no name
        {
            // Add void to the global scope
            UPVoid        typeVoidPtr(new Void);
            Void&         typeVoid = add<Void>();

            // Add "Std" namespace to the global scope.
            // Here we have the standard basic types.
            Namespace&    standard = add<Namespace>("Std");
            Class&        typeInt = standard.add<Class>("Integer");
            Class&        typeString = standard.add<Class>("String");
            ((void)typeInt);

            // Add "Sys" namespace to the global scope.
            // This has object to interact with the computer.
            //  console
            ParamList     consoleParamList;
            consoleParamList.emplace_back(typeString);

            Namespace&  system                  = add<Namespace>("Sys");

            Class&      consoleType             = system.add<Class>("Console");
            /*Object&     consoleObject           = */ system.add<Object>("console", consoleType);

            Func&       consolePrintMethodType  = consoleType.add<Func>("Print", consoleParamList, typeVoid);
            /*Object&     consolePrintMethodFunc  = */ consoleType.add<Object>("print", consolePrintMethodType);


            // Special scope to hold all literals.
            /*Namespace&  literal                 = */ add<Namespace>("$Literal");
        }
};

}

#endif
