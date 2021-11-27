#include "FireVM.h"

#include <iostream>
#include <fstream>

int main(int argc, char* /*argv*/[])
{
    if (argc != 2)
    {
        std::cerr << "Invalid Invocation of VM\n";
        return 1;
    }
/*
    std::cout << "Starting Fire VM\n";

    FireVM vm(32768)

    std::ifstream   file(argv[1]);
    vm.load(file);

    vm.run();
*/
}
