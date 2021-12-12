#ifndef THORSANVIL_ANVIL_LIB_SYSTEM_CONSOLE_H
#define THORSANVIL_ANVIL_LIB_SYSTEM_CONSOLE_H

#include <string>

#ifdef ANVIL
#pragma vera-pushoff
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

    $Destructor:
    {->
    <-}

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
#pragma vera-pop
#else


namespace Ice::System
{

    void print(std::string const& value);

}
#endif

#endif
