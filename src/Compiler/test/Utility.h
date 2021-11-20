#ifndef TEST_COMPILE_ACTION
#define TEST_COMPILE_ACTION

#include <sstream>


#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_TRUE(check);         \
                                        } catch(...) {                  \
                                            std::cerr << "TRUE: FAIL\n";\
                                            std::cerr << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }
#define EXPECT_FALSE_OR_DEBUG(check, output) \
                                     {                                  \
                                        try {                           \
                                            EXPECT_FALSE(check);        \
                                        } catch(...) {                  \
                                            std::cerr << "FALSE FAIL\n";\
                                            std::cerr << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }
#define EXPECT_THROW_OR_DEBUG(check, msg, output) \
                                     {                                  \
                                        try {                           \
                                            check;                      \
                                            std::cerr << "THROW FAIL (Throw expected but did not happen)\n";\
                                            std::cerr << output.rdbuf();\
                                        } catch(std::exception const& e)\
                                        {                               \
                                            std::string error(e.what());\
                                            if (error.find(msg) == std::string::npos) {\
                                                std::cerr << "THROW FAIL (Throw happened but not expected message)\n" \
                                                          << error << "\n"  \
                                                          << output.rdbuf();\
                                            }                           \
                                        }                               \
                                      }

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}

#endif
