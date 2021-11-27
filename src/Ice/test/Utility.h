#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

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
                                            EXPECT_TRUE(false);         \
                                        }                               \
                                        catch(std::exception const& e)  \
                                        {                               \
                                            std::string error(e.what());\
                                            if (error.find(msg) == std::string::npos) {\
                                                std::cerr << "THROW FAIL (Throw happened but not expected message)\n" \
                                                          << error << "\n"  \
                                                          << output.rdbuf();\
                                                throw;                  \
                                            }                           \
                                        }                               \
                                        catch(std::string const& m)     \
                                        {                               \
                                            std::cerr << "THROW FAIL (Got a string expecting a standard exception)\n" \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(int e)                    \
                                        {                               \
                                            std::cerr << "THROW FAIL (Got an int expecting a standard exception)\n" \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(...)                      \
                                        {                               \
                                            std::cerr << "THROW FAIL\n" \
                                                      << "This was a non standard exception of unknown type\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }
#define EXPECT_NO_THROW_OR_DEBUG(check, output) \
                                     {                                  \
                                        try                             \
                                        {                               \
                                            check;                      \
                                            EXPECT_TRUE(true);          \
                                        }                               \
                                        catch(std::exception const& e)\
                                        {                               \
                                            std::string error(e.what());\
                                            std::cerr << "THROW FAIL\n" \
                                                      << "Exception Message: " << error << "\n"  \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(std::string const& m)     \
                                        {                               \
                                            std::cerr << "THROW FAIL\n" \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(int e)                    \
                                        {                               \
                                            std::cerr << "THROW FAIL\n" \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(...)                      \
                                        {                               \
                                            std::cerr << "THROW FAIL\n" \
                                                      << "This was a non standard exception of unknown type\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                      }

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}


#endif
