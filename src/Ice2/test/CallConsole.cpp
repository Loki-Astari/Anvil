#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Storage.h"
#include "Declaration.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

struct FacadeCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Namespace       globalScope;
    ThorsAnvil::Anvil::Ice::Storage         storage;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    FacadeCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, globalScope, storage, output)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(CallConsole, CallPrintOnConsole)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
}
namespace Sys
{
    class Console
    {
        library  : Void;
        function : Void;

        construct{}
        {
            <--
                ADDR LRR Expr-1 = This + 0
                ADDR LRR Expr-2 = This + 1
                CMD Import Load Expr-1 Anvil_System
                CMD Import GetSymbol Expr-1 Expr-2 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
            -->
        }
        destruct{}
        {
            <--
                ADDR LRR Expr-1 = This + 0
                CMD Import UnLoad Expr-1
            -->
        }

        func Print{Std::String -> Void}
        print : Print {
            <--
                ADDR LRR Expr-1 = This + 1
                ADDR LRR Expr-2 = FramePointer - 5
                CMD Import Call Expr-1 Expr-1
            -->
        }
    }
    console : Console;
}
namespace Name_Space
{
    main : func {Void -> Std::Integer}
    {
        Sys::console.print("Hello World");
        return 0;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
