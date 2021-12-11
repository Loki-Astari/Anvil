#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>
#include <dlfcn.h>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionCMD(Instruction instruction)
{
    switch (instruction & Assembler::Action_CMD_Mask)
    {
        case Assembler::Cmd_NoOp:       {return; /*Do Nothing*/}
        case Assembler::Cmd_Kill:       {return runActionCMDKill(instruction);}
        case Assembler::Cmd_InitShort:  {return runActionCMDInit(instruction);}
        case Assembler::Cmd_InitLarge:  {return runActionCMDInit(instruction);}
        case Assembler::Cmd_Import:     {return runActionCMDImport(instruction);}
        default:
        {
            running = false;
            result = haltInvalidCmd;
        }
    }
}

void FireVM::runActionCMDKill(Instruction instruction)
{
    running = false;
    result = instruction & Assembler::Cmd_Kill_Result_Mask;
}

void FireVM::runActionCMDInit(Instruction instruction)
{
    std::uint32_t globalSize = instruction & 0xFF;

    if (instruction & Assembler::Cmd_Init_GlobalMask)
    {
        ++state.programCounter;
        globalSize = memory[state.programCounter];
    }

    ++state.programCounter;
    std::uint32_t stackSize  = memory[state.programCounter];
    if (instruction & Assembler::Cmd_Init_StackMask)
    {
        ++state.programCounter;
        stackSize = (stackSize << 16) | memory[state.programCounter];
    }
    globalSize = std::max(static_cast<std::uint32_t>(1), globalSize);
    stackSize  = std::max(static_cast<std::uint32_t>(1), stackSize);

    state.global.resize(globalSize);
    state.stack.resize(stackSize);

    std::uint32_t oldSize = state.addressRegister[StackP] -  state.addressRegister[FrameP];
    oldSize = std::min(oldSize, stackSize);

    state.addressRegister[Global] = &state.global[0];
    state.addressRegister[FrameP] = &state.stack[oldSize];
    state.addressRegister[StackP] = &state.stack[0];
}

void FireVM::runActionCMDImport(Instruction instruction)
{
    switch (instruction & Assembler::Cmd_Import_Mask)
    {
        case Assembler::Cmd_Import_Load:        {return runActionCMDImportLoad(instruction);}
        case Assembler::Cmd_Import_GetSymbol:   {return runActionCMDImportGetSymbol(instruction);}
        case Assembler::Cmd_Import_Call:        {return runActionCMDImportCall(instruction);}
        case Assembler::Cmd_Import_UnLoad:
        {
            std::cerr << "UnLoad\n";
            std::cerr << "==================\n";
            break;
        }
        default:
        {
            running = false;
            result = haltInvalidCmdLoadType;
        }
    }
}

void FireVM::runActionCMDImportLoad(Instruction instruction)
{
    int reg = (instruction & Assembler::Cmd_Import_Reg1_Mask) >> Assembler::Cmd_Import_Reg1_Shift;

    ++state.programCounter;
    int libNameSize = memory[state.programCounter];
    ++state.programCounter;
    char const* libNamePtr = reinterpret_cast<char const*>(&memory[state.programCounter]);
    std::string libName("/usr/local/lib/Anvil/lib");
    libName += std::string(libNamePtr, libNameSize);
    libName += ".dylib";
    libNameSize = (libNameSize + ((libNameSize % 2) == 1 ? 1 : 0)) / 2;
    state.programCounter += libNameSize;
    --state.programCounter; // Remember the loop will automatically increment the counter.

    void* library = dlopen(libName.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (library == nullptr)
    {
        running = false;
        result = haltInvalidImportLoadBadLibrary;
    }

    state.addressRegister[reg]->init(CodeAddress(library));
}

void FireVM::runActionCMDImportGetSymbol(Instruction instruction)
{
    int dstReg = (instruction & Assembler::Cmd_Import_Reg1_Mask) >> Assembler::Cmd_Import_Reg1_Shift;
    int dllInfo = (instruction & Assembler::Cmd_Import_Reg2_Mask) >> Assembler::Cmd_Import_Reg2_Shift;

    ++state.programCounter;
    int symbolNameSize = memory[state.programCounter];
    ++state.programCounter;
    char const* symbolNamePtr = reinterpret_cast<char const*>(&memory[state.programCounter]);
    std::string symbolName(symbolNamePtr, symbolNameSize);
    symbolNameSize = (symbolNameSize + ((symbolNameSize % 2) == 1 ? 1 : 0)) / 2;
    state.programCounter += symbolNameSize;
    --state.programCounter; // Remember the loop will automatically increment the counter.

    bool test = true;
    CodeAddress library = state.addressRegister[dllInfo]->getValue<CodeAddress>([&](){test = false;});
    void* symbol = dlsym(reinterpret_cast<void*>(library), symbolName.c_str());
    if (symbol == nullptr)
    {
        running = false;
        result = haltInvalidImportGetSymbolFailed;;
    }

    state.addressRegister[dstReg]->init(CodeAddress(symbol));
}

void FireVM::runActionCMDImportCall(Instruction instruction)
{
    int funcReg     = (instruction & Assembler::Cmd_Import_Reg1_Mask) >> Assembler::Cmd_Import_Reg1_Shift;
    int paramReg    = (instruction & Assembler::Cmd_Import_Reg2_Mask) >> Assembler::Cmd_Import_Reg2_Shift;

    using FuncPointer = void (*)(std::string const&);
    std::string& param = (state.addressRegister[paramReg] + 1)->getValue<String>([&](){running = false; result = haltInvalidImportNotFunc;});

    CodeAddress funcCode = state.addressRegister[funcReg]->getValue<CodeAddress>([&](){running = false; result = haltInvalidImportNotFunc;});
    void* funcVPoint = reinterpret_cast<void*>(funcCode);
    FuncPointer funcPointer = reinterpret_cast<FuncPointer>(funcVPoint);
    (*funcPointer)(param);
}
