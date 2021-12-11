#ifndef THORSANVIL_ANVIL_LIB_SYSTEM_CONSOLE_H
#define THORSANVIL_ANVIL_LIB_SYSTEM_CONSOLE_H

#include <string>

#ifdef ANVIL
#pragma vera-pushoff
namespace System
{
    constructor:
    {->
        # When a function is called the
        #       "Stack" pointer is set.
        #       "This"  pointer is null
        # Objects at offset from the "Stack" pointer are:
        #   0:          Location to put the return value
        #   1:          Previous Stack Pointer
        #   2:          Previous This  Pointer
        #   3:          The pc to set when returning from the function.
        #   [4->n+4)    Any parameters passed to the function.
        #
        # When a namespace constructor is called one parameter is passed.
        #   The address of the first object of that namespace within the global region.
        #   In this case the "console" object.

        # To initiale the "console" object.
        # We must create the object then call its constructor.

            # To create a class object we create a Frame object in the VM.
            # Then call the constructor
            Set Stack + 4 = Frame 2

            # Call the constructor set up the next stack frame
            # Call 5 Method Stack + 4 !System::Console::$constructor
            Call 5 Method Stack + 4 !System::Console::constructor

        # Now return
        Return

    <-}
    destructor:
    {->
        # See comments for constructor.

        # Call Method Stack + 4 !System::Console::$constructor
        Call 5 Method Stack + 4 !System::Console::destructor

        Set Stack + 4 = Void

        # Now return
        Return
    <-}
    class Console
    {
        constructor: func
        {->
            # When a method is called the
            #       "Stack" pointer is set.
            #       "This"  pointer points at the object
            # Objects at offset from the "Stack" pointer are:
            #   0:          Location to put the return value
            #   1:          Previous Stack Pointer
            #   2:          Previous This  Pointer
            #   3:          The pc to set when returning from the function.
            #   [4->n+4)    Any parameters passed to the function.
            #
            # Objects at offset from the "This" pointer are:
            #   0:          Parent object
            #   [1->n+1)    All the member variables

            # Loads the result of dlopen() into Global + $obj1
            CMD Import Add 4 Anvil_System
            Set This + 1 = Stack + 4

            # Loads the result of dlsym() into  Global + $obj2
            Set Stack + 8 = This + 1
            CMD Import Sym 4 __ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
            Set This + 2 = Stack + 4

            # Done
            Return
        <-}
        destructor:
        {->
            # See comments for constructor.

            # Reads the pointer from Global + $obj1 calls dlclose()
            Set Stack + 8 = This + 1
            CMD Import Rem 4
        <-}
        # lib is unusable by the "Anvil" language as it has type "Void"
        # But still takes a space in the object that can be used by the assembler.
        # The constructor above uses this to hold a pointer to the dynamicall loaded module
        lib: Void;

        # This is a function that can be used.
        print : func {System::String => Void}
        {->
            # When a method is called the
            #       "Stack" pointer is set.
            #       "This"  pointer points at the object
            # Objects at offset from the "Stack" pointer are:
            #   0:          Location to put the return value
            #   1:          Previous Stack Pointer
            #   2:          Previous This  Pointer
            #   3:          The pc to set when returning from the function.
            #   [4->n+4)    Any parameters passed to the function.
            #
            # Objects at offset from the "This" pointer are:
            #   0:          Parent object
            #   [1->n+1)    All the member variables

            Set Stack + 9 = Stack + 1
            Cmd Import Call 5 String

            # Done
            Return
        <-}
    }
    console: Conole;
}
#pragma vera-pop
#else


namespace Ice::System
{

    void print(std::string const& value);

}
#endif

#endif
