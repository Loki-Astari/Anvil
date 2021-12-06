
#include "Assembler.h"
#include "FireVM.h"

template<typename Output = ThorsAnvil::Anvil::Fire::CodeBlock>
struct BuildAssembler
{
    ThorsAnvil::Anvil::Fire::SymbolTable         stable;
    Output                                       codeBlock;
    ThorsAnvil::Anvil::Fire::Assembler           assembler;

    BuildAssembler(std::ostream& result, std::string&& input)
        : assembler(result, stable)
    {
        std::istringstream      inputStream(std::move(input));
        assembler.assemble(inputStream, codeBlock);
    }
};

struct BuildVM
{
    ThorsAnvil::Anvil::Fire::VMState             machineState;
    ThorsAnvil::Anvil::Fire::CodeBlock           codeBlock;
    ThorsAnvil::Anvil::Fire::FireVM              vm;

    BuildVM(std::ostream& result, std::string&& input)
        : vm(machineState, codeBlock)
    {
        std::istringstream                      inputStream(std::move(input));
        ThorsAnvil::Anvil::Fire::SymbolTable    stable;
        ThorsAnvil::Anvil::Fire::Assembler      assembler(result, stable);

        assembler.assemble(inputStream, codeBlock);
    }

    ThorsAnvil::Anvil::Fire::Result run()
    {
        return vm.run();
    }
};


