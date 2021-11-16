#include "Anvil.h"
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <map>


using Action = std::function<void(std::vector<std::string> const&)>;

void run(std::vector<std::string> const& /*args*/)
{
    std::cout << "Run Virtual Machine\n";
}

void compile(std::vector<std::string> const& /*args*/)
{
    std::cout << "Run Compiler\n";
}

void usage(std::vector<std::string> const& /*args*/ = {})
{
    std::cerr << "Usage\n";
    exit(1);
}

int main(int argc, char* argv[])
{
    const std::map<std::string, Action> action =
    {
         {"usage",   usage}
        ,{"run",     run}
        ,{"compile", compile}
    };


    std::vector<std::string>    args(argv, argv + argc);

    std::cout << "Anvil: V 1.0\n";
    if (args.size() < 2) {
        usage();
    }

    auto command = action.find(args[1]);
    if (command == action.end()) {
        usage();
    }

    command->second(args);
}
