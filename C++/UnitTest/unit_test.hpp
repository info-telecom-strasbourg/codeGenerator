#ifndef ITS_UNIT_TEST_H
#define ITS_UNIT_TEST_H

#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

/* Macros for colors */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"

/**
 * Macro to print an error message 
 * @param expr: the expression is false (failed the test)
 */
#define test_error(expr) std::cout << COLOR_RED << "Failed " << COLOR_RESET  << std::endl << "assertion failed : " << expr << std::endl;

/**
 * Macro to execute a test
 * @param func: 
 */
#define TEST(func)                                                            \
    do                                                                        \
    {                                                                         \
        __current_test_name = #func;                                          \
        std::cout << "Test of the function " << __current_test_name << " : "; \
        loading = true;                                                       \
        std::thread load (loadingEffect);                                     \
        func();                                                               \
        loading = false;                                                      \
        load.join();                                                          \
        std::cout << COLOR_GREEN << "Done" << COLOR_RESET << std::endl;       \
    } while (0)

#define assert(expr)                                  \
    do                                                \
    {                                                 \
        if (!(expr))                                  \
        {                                             \
            test_error(#expr);                        \
            exit(1);                                  \
        }                                             \
    } while (0)

static std::string __current_test_name;
static bool loading;

static void loadingEffect()
{
    std::string loadChar = "/|\\-";
    int ind = 0;
    while (loading)
    {
        ind = (ind + 1) % 4;
        std::cout << "\b" << loadChar[ind];
        fflush(stdout);
        usleep(150000);
    }
    std::cout << "\b" << " ";
    fflush(stdout);
}

#endif
