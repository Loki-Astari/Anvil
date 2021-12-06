
#include "Assembler.h"
#include "FireVM.h"
#include "../Ice/test/Utility.h"

template<typename Output = ThorsAnvil::Anvil::Fire::CodeBlock>
struct BuildAssembler
{
    ThorsAnvil::Anvil::Fire::SymbolTable        stable;
    Output                                      codeBlock;
    ThorsAnvil::Anvil::Fire::Assembler          assembler;
    bool                                        bad;

    BuildAssembler(std::ostream& result, bool& bad, std::string&& input)
        : BuildAssembler(true, result, bad, std::move(input))
    {}

    BuildAssembler(bool good, std::ostream& result, bool& bad, std::string&& input)
        : assembler(result, stable)
    {
        std::istringstream      inputStream(std::move(input));
        assembler.assemble(inputStream, codeBlock);

        EXPECT_EQ_OR_LOG(bad, assembler.isOK(), good, result);
    }
};

struct BuildVM
{
    ThorsAnvil::Anvil::Fire::VMState            machineState;
    ThorsAnvil::Anvil::Fire::CodeBlock          codeBlock;
    ThorsAnvil::Anvil::Fire::FireVM             vm;
    ThorsAnvil::Anvil::Fire::SymbolTable        stable;

    BuildVM(std::ostream& result, bool& bad, std::string&& input)
        : vm(machineState, codeBlock)
    {
        machineState.global.resize(100);
        machineState.stack.resize(100);

        // Technicall we don't need this as the assembler will
        // auto add CMD Init if not included.
        // BUT: we set the size to 100 with the resizes above.
        //      we using the same size here in the init no
        //      sub sequent resize will be done and we can get
        //      get the address of global and stack locations
        //      before we call run() to help set up the tests.
        std::string                             init = "CMD Init 100 100\n";
        std::istringstream                      inputStream(init + input);
        ThorsAnvil::Anvil::Fire::Assembler      assembler(result, stable);

        assembler.assemble(inputStream, codeBlock);

        EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    }

    ThorsAnvil::Anvil::Fire::Result run()
    {
        return vm.run();
    }
};


