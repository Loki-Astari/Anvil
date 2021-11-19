#ifndef TEST_COMPILE_ACTION
#define TEST_COMPILE_ACTION

#include "Action.h"
#include <sstream>


class CompileAction: public ThorsAnvil::Anvil::Ice::Action
{
    std::stringstream   log;
    public:
        CompileAction()
            : Action(log)
        {}
        ~CompileAction() {}

        void display(std::ostream& out)
        {
            out << log.str();
        }
};

#endif
