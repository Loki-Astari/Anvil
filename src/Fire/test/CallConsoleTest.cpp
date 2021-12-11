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
        ADDR LRR Expr-1 = This + 1
        CMD Import Load Expr-1 Anvil.System
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
CMD Init 1000 1000                                  // 2

// Initialize All Namespaces
ADDR LRR Expr-1 = Global + 0                        // 5
JUMP Call Expr-1 NS_Std_$Constructor                // 6

ADDR LRR Expr-1 = Global + 0                        // 9
JUMP Call Expr-1 NS_Sys_$Constructor                // 10

ADDR LRR Expr-1 = Global + 2                        // 13
JUMP Call Expr-1 NS_MyApp_$Constructor              // 15

// Run the applicaiton:
ADDR INC  StackPointer 1                            // 18 Make Space for the return parameter
JUMP Call Expr-1 NS_MyApp_Func_main                 // 19
ADDR LRR  Expr-1 = StackPointer + 0
ADDR DEC  StackPointer 1
CMD Kill 14                                          // Need to support extract result from register or memory

NS_Std_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL                                      // 25

NS_Sys_$Constructor:
// Initialize the 'console' object
    ADDR LRR Expr-1 = This + 1                      // 26
    CMD Import Load Expr-1 Anvil_System             // 28

    // Set the location pointed at by Expr-1 (console) to a Frame (size 1 method print)
    ADDR LRR Expr-1 = This + 0                      // 36
    ADDR LML Expr-1 = DataFrame 2                   // 37
    ADDR LRP Expr-1 = Expr-1 + 0                    // 39
    // Call the Console constructor
    // Setting the This register to the value of Expr-1
    JUMP Call Expr-1 NS_Sys_Class_Console_$Constructor  // 40
JUMP Return AL                                      // 


NS_Sys_Class_Console_$Constructor:
// Construct an object of type Console
    ADDR LRR Expr-1 = This + 0                      // 44
    ADDR LML Expr-1 = CodeAddress NS_Sys_Class_Console_Func_print     // 45
    ADDR INC Expr-1 1                               // 48
    ADDR LRR Expr-2 = Global + 1                    // 49
    CMD Import GetSymbol Expr-1 Expr-2 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
JUMP Return AL

NS_Sys_Class_Console_Func_print:
    ADDR LRR Expr-1 = This + 1                      // Location of function pointer used by Import Call.
    ADDR LRR Expr-2 = StackPointer + 0              // Location of the return value and input parameters.
    ADDR DEC Expr-2 5
    CMD Import Call Expr-1 Expr-2                   // Call the stored pointer.
JUMP Return AL


NS_MyApp_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL                                      // 55

NS_MyApp_Func_main:

// Sys::console.print("Hello World");
ADDR LRP  Expr-1 = Global + 0                       // 56 Expr-1 points at Sys::console
ADDR INC  StackPointer 1                            // 57 Make Space for the return parameter
ADDR LML  StackPointer = String  Hello World        // 58 Push the only parameter.
ADDR INC  StackPointer 1                            // Increment the Stack pointer
JUMP Method Expr-1 0                                // 68 Expr-1 becomes this 0 is the location in the object of the method
ADDR DEC StackPointer 2                             // Pop Param-0 and result

// return 3;
ADDR LRR Expr-1 = FramePointer + 0
ADDR DEC Expr-1 5                               // Point at the result location.
ADDR LML Expr-1 = Int 3                         // Set the return value
JUMP Return AL


)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 14, result);
    EXPECT_SUCC(bad, result);
}

