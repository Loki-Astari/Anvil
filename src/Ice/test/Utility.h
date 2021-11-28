#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include <sstream>

#define EXPECT_EQ_OR_LOG(save, test, value, output) \
                                     {                                  \
                                        if ((test) != (value)) {        \
                                            save = true;                \
                                            output << #test << "(" << (test) << ")  != " << #value << "(" << (value) << ")\n";   \
                                        }                               \
                                    }
#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     {                                  \
                                        EXPECT_TRUE(check);             \
                                        if (!check) {                   \
                                            std::cout << "TRUE: FAIL\n";\
                                            std::cout << output.rdbuf();\
                                        }                               \
                                      }
#define EXPECT_FALSE_OR_DEBUG(check, output) \
                                     {                                  \
                                       EXPECT_FALSE(check);             \
                                       if (check) {                     \
                                            std::cout << "FALSE FAIL\n";\
                                            std::cout << output.rdbuf();\
                                        }                               \
                                      }
#define EXPECT_THROW_OR_DEBUG(check, msg, output) \
                                     {                                  \
                                        try {                           \
                                            check;                      \
                                            std::cout << "THROW FAIL (Throw expected but did not happen)\n";\
                                            std::cout << output.rdbuf();\
                                            EXPECT_TRUE(false);         \
                                        }                               \
                                        catch(std::exception const& e)  \
                                        {                               \
                                            std::string error(e.what());\
                                            if (error.find(msg) == std::string::npos) {\
                                                std::cout << "THROW FAIL (Throw happened but not expected message)\n" \
                                                          << error << "\n"  \
                                                          << output.rdbuf();\
                                                throw;                  \
                                            }                           \
                                        }                               \
                                        catch(std::string const& m)     \
                                        {                               \
                                            std::cout << "THROW FAIL (Got a string expecting a standard exception)\n" \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(int e)                    \
                                        {                               \
                                            std::cout << "THROW FAIL (Got an int expecting a standard exception)\n" \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(...)                      \
                                        {                               \
                                            std::cout << "THROW FAIL\n" \
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
                                            std::cout << "THROW FAIL\n" \
                                                      << "Exception Message: " << error << "\n"  \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(std::string const& m)     \
                                        {                               \
                                            std::cout << "THROW FAIL\n" \
                                                      << "String Thrown: " << m << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(int e)                    \
                                        {                               \
                                            std::cout << "THROW FAIL\n" \
                                                      << "Int Thrown: " << e << "\n" \
                                                      << output.rdbuf();\
                                            throw;                      \
                                        }                               \
                                        catch(...)                      \
                                        {                               \
                                            std::cout << "THROW FAIL\n" \
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
