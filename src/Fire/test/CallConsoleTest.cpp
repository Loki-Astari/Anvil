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
    // Compiler generated constructor for class
    // Run at startup.
    $Constructor: func{Void -> Void}
    {
        // Compiler generated Full Label: NS_Std_$Constructor
    }

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
    // Compiler generated constructor for class
    // Run at startup.
    $Constructor: func{Void -> Void}
    {
        ->
        // Compiler generated Full Label: NS_Sys_$Constructor:

        // Initialize the 'console' object
        ADDR LRR Expr-1 = This + 1
        CMD Import Load Expr-1 Anvil_System

        // Set the location pointed at by Expr-1 (console) to a Frame (size 1 method print)
        ADDR LRR Expr-1 = This + 0
        ADDR LML Expr-1 = DataFrame 2
        ADDR LRP Expr-1 = Expr-1 + 0
        // Call the Console constructor
        // Setting the This register to the value of Expr-1
        JUMP Call Expr-1 NS_Sys_Class_Console_$Constructor
        <-
    }


    class Console
    {
        // Compiler generated constructor for class
        // Run when object of this class created.
        $Constructor: func{Void -> Void}
        {
            ->
            // Compiler generated Full Label: NS_Sys_Class_Console_$Constructor:

                // Get the address of the print function (it is a member of the class)
                ADDR LRR Expr-1 = This + 0
                ADDR LML Expr-1 = CodeAddress NS_Sys_Class_Console_Func_print

                // Get the address of the C++ function that print will use.
                ADDR INC Expr-1 1
                ADDR LRR Expr-2 = Global + 1
                CMD Import GetSymbol Expr-1 Expr-2 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
            <-
        }

        print: func {Void -> Std::String}
        {
            ->
                // Compiler generated Full label: NS_Sys_Class_Console_Func_print:
                // Expr-1 points at the address of the C++ function.
                ADDR LRR Expr-1 = This + 1

                // Expr-2 points at the return value (Note Parameters are after that)
                ADDR LRR Expr-2 = StackPointer + 0
                ADDR DEC Expr-2 5

                // Call C++ function.
                CMD Import Call Expr-1 Expr-2
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
    // Compiler generated constructor for class
    // Run at startup.
    $Constructor: func{Void -> Void}
    {
        // Compiler generated Full label: CNS_MyApp_$Constructor:
        // No Objects.
        // We don't need to do anything.
    }

NS_MyApp_Func_main:


JUMP Return AL
    main: func { Void -> Std::Int }
    {
        Sys::console.print("Hello World");
            // ADDR LRP  Expr-1 = Global + 0                // The console object lives here.
            // ADDR INC  StackPointer 1                     // Increment to leave space for return
            // ADDR LML  StackPointer = String  Hello World // Add the parameter needed
            // ADDR INC  StackPointer 1                     // And increment.
            // JUMP Method Expr-1 0                         // Call the method (print is value 0)
            ADDR DEC StackPointer 2                         // Remove the return value and parameter from stack

        return 3;
            // ADDR LRR Expr-1 = FramePointer + 0           // Get the pointer to the return area.
            // ADDR DEC Expr-1 5
            // ADDR LML Expr-1 = Int 3                      // Load 3 into this location.
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
ADDR LRR Expr-1 = Global + 0
JUMP Call Expr-1 NS_Std_$Constructor

ADDR LRR Expr-1 = Global + 0
JUMP Call Expr-1 NS_Sys_$Constructor

ADDR LRR Expr-1 = Global + 2
JUMP Call Expr-1 NS_MyApp_$Constructor

// Run the applicaiton
ADDR INC  StackPointer 1
JUMP Call Expr-1 NS_MyApp_Func_main
ADDR LRR  Expr-1 = StackPointer + 0
ADDR DEC  StackPointer 1
CMD Kill 14

NS_Std_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL

NS_Sys_$Constructor:
// Initialize the 'console' object
    ADDR LRR Expr-1 = This + 1
    CMD Import Load Expr-1 Anvil_System

    // Set the location pointed at by Expr-1 (console) to a Frame (size 1 method print)
    ADDR LRR Expr-1 = This + 0
    ADDR LML Expr-1 = DataFrame 2
    ADDR LRP Expr-1 = Expr-1 + 0
    // Call the Console constructor
    // Setting the This register to the value of Expr-1
    JUMP Call Expr-1 NS_Sys_Class_Console_$Constructor
JUMP Return AL


NS_Sys_Class_Console_$Constructor:
// Construct an object of type Console
    ADDR LRR Expr-1 = This + 0
    ADDR LML Expr-1 = CodeAddress NS_Sys_Class_Console_Func_print
    ADDR INC Expr-1 1
    ADDR LRR Expr-2 = Global + 1
    CMD Import GetSymbol Expr-1 Expr-2 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
JUMP Return AL

NS_Sys_Class_Console_Func_print:
    ADDR LRR Expr-1 = This + 1
    ADDR LRR Expr-2 = StackPointer + 0
    ADDR DEC Expr-2 5
    CMD Import Call Expr-1 Expr-2
JUMP Return AL


NS_MyApp_$Constructor:
// No Objects.
// We don't need to do anything.
JUMP Return AL

NS_MyApp_Func_main:

// Sys::console.print("Hello World");
ADDR LRP  Expr-1 = Global + 0
ADDR INC  StackPointer 1
ADDR LML  StackPointer = String  Hello World
ADDR INC  StackPointer 1
JUMP Method Expr-1 0
ADDR DEC StackPointer 2

// return 3;
ADDR LRR Expr-1 = FramePointer + 0
ADDR DEC Expr-1 5
ADDR LML Expr-1 = Int 3
JUMP Return AL


)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 14, result);
    EXPECT_SUCC(bad, result);
}

