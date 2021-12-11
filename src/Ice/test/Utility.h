#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include <sstream>

#define EXPECT_EQ_OR_LOG(save, test, value, output) \
                                    do                                  \
                                    {                                   \
                                        if ((test) != (value)) {        \
                                            save = true;                \
                                            output << #test << "(" << (test) << ")  != " << #value << "(" << (value) << ")\n";   \
                                        }                               \
                                    }                                   \
                                    while(false)
// EXPECT_EQ_OR_LOG set the check value to true on exit.
// So a "Successes" means that the check value remains
// false at the end of the tests.
#define EXPECT_SUCC(check, output)      EXPECT_FALSE_OR_DEBUG(check, output)

// If we expect failure.
// Then check we have failed and that we have the correct error message.
#define EXPECT_FAIL(check, error, output)   \
                                    do                                          \
                                    {                                           \
                                        EXPECT_TRUE((check == true) && (output.str().find(error) != std::string::npos));    \
                                        if ((check != true) || (output.str().find(error) == std::string::npos)) {           \
                                            std::cout << "TRUE: FAIL\n";        \
                                            std::cout << output.str() << "\n";  \
                                        }                                       \
                                    }                                           \
                                    while(false)

#define EXPECT_TRUE_OR_DEBUG(check, output) \
                                     do                                 \
                                     {                                  \
                                        EXPECT_TRUE(check);             \
                                        if (!(check)) {                 \
                                            std::cerr << "TRUE: FAIL\n";\
                                            std::cerr << output.rdbuf();\
                                        }                               \
                                      }                                 \
                                      while(false)
#define EXPECT_FALSE_OR_DEBUG(check, output) \
                                     do                                 \
                                     {                                  \
                                       EXPECT_FALSE(check);             \
                                       if (check) {                     \
                                            std::cerr << "FALSE FAIL\n";\
                                            std::cerr << output.str();  \
                                        }                               \
                                      }                                 \
                                      while(false)
#define EXPECT_THROW_OR_DEBUG(check, msg, output) \
                                     do                                 \
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
                                      }                                 \
                                      while(false)
#define EXPECT_NO_THROW_OR_DEBUG(check, output) \
                                     do                                 \
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
                                      }                                 \
                                      while(false)

inline std::stringstream buildStream(char const* buffer)
{
    std::stringstream stream;
    stream << buffer;
    return stream;
}


#endif
