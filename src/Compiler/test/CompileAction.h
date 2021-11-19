#ifndef TEST_COMPILE_ACTION
#define TEST_COMPILE_ACTION

#include "Action.h"
#include <sstream>


#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_TRUE(check);         \
                                        } catch(...) {                  \
                                            output.display(std::cout);  \
                                            throw;                      \
                                        }                               \
                                      }

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}

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
