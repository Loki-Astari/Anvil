#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(CallConsoleTest, CallConsole)
{
#if 0

## File: Std.ice
namespace Std
{
    class Integer
    {
        // Definition of Int
    }
    class String
    {
        // Definition of String
    }
}

## File: Sys.ice
namespace Sys
{
    constructor: func{Void -> Void}
    {
        ->
        CMD Import Load $dll Anvil.System
        <-
    }
    class Console
    {
        print: func {Void -> Std::String}
        {
            ->
            CMD Import Call $dll SymbolForDll
            <-
        }
    }
    console: Console;
    $dll: Void;
}
## File: MyApp.ice
------------------
import Std;
import Sys;

namespace MyApp
{
    main: func { Void -> Std::Int }
    {
        Sys::console.print("Hello World");
        return 3;
    }
}

## Execute
anvil MyApp.ice

#endif

    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 1000 1000

// Initialize All Namespaces
LOAD LRR Expr-1 Global + 0
JUMP Call AL Abs Expr-1 NS_Std_$Constructor

LOAD LRR Expr-1 Global + 0
JUMP Call AL Abs Expr-1 NS_Sys_$Constructor

LOAD LRR Expr-1 Global + 2
JUMP Call AL Abs Expr-1 NS_MyApp_$Constructor

// Run the applicaiton:
REG  Inc  StackP 1                          // Make Space for the return parameter
JUMP Call AL Abs null NS_MyApp_Func_main
LOAD LRP  Expr-1 = StackP + 0
REG  POP StackP
CMD Kill Reg Expr-1

NS_Std_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL

NS_Sys_$Costructor:
// Initialize the 'console' object
    // Set the location pointed at by Expr-1 (console) to a Frame (size 1 method print)
    LOAD LRR Expr-1 = This + 0
    LOAD LMV Expr-1 = Frame(1)
    LOAD LRP Expr-1 = Expr-1 + 0
    // Call the Console constructor
    // Setting the This register to the value of Expr-1
    JUMP Call AL Abs Expr-1 NS_Sys_Class_Console_$Constructor

    CMD Import Load $dll Anvil.System
JUMP Return AL


NS_Sys_Class_Console_$Constructor:
// Construct an object of type Console
    LOAD LMV THIS + 0 = CodeAddress(NS_Sys_Class_Console_Func_print)
JUMP Return AL

NS_Sys_Class_Console_Func_print:
    LOAD LRR Expr-1 = StackP - 5
    LOAD LRV Expr-2 = 5
    CMD Import Call $dll SymbolForDll Expr-1 Expr-2
JUMP Return AL


NS_MyApp_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL

NS_MyApp_Func_main:

// Sys::console.print("Hello World");
LOAD LRR Expr-1 = Global + 0                // Expr-1 points at Sys::console
LOAD LRP Expr-2 = Expr-1 + 0                // Expr-2 points at print method in object
REG  Inc  StackP 1                          // Make Space for the return parameter
REG  Push StackP String("Hello World")      // Push the only parameter.
JUMP Call AL Mem Expr-1 Expr-2              // Expr-1 becomes this. Mem Expr-2 points at function
REG  POP StackP                             // Pop Param-0
REG  POP StackP                             // Pop the result

// return 3;
LOAD LRR Expr-1 = FrameP - 5                // Point at the result location.
LOAD LMV Expr-1 = Int(3)                    // Set the return value
JUMP Return AL


)");

    // Result      output = vm.run();

    // EXPECT_EQ_OR_LOG(bad, output, 255, result);
    // EXPECT_FALSE_OR_DEBUG(bad, result);
}

