
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

        std::istringstream                      inputStream(std::move(input));
        ThorsAnvil::Anvil::Fire::Assembler      assembler(result, stable);

        assembler.assemble(inputStream, codeBlock);

        EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    }

    ThorsAnvil::Anvil::Fire::Result run()
    {
        return vm.run();
    }
};


