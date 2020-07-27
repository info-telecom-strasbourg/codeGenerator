#ifndef ITS_UNIT_TEST_H
#define ITS_UNIT_TEST_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <time.h>
#include <chrono>
#include <future>

/* Macros for colors */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"

/**
 * Macro to print an error message 
 * @param expr: the expression is false (failed the test)
 */
#define test_error(expr) std::cout << COLOR_RED << "Failed " << COLOR_RESET  << std::endl << "assertion failed : " << expr << std::endl; fflush(stdout);

/**
 * Macro to execute a test
 * @param func: the tested function 
 */
#define TEST(func)                                                                                               \
    do                                                                                                           \
    {                                                                                                            \
        std::string __current_test_name = #func;                                                                 \
        std::cout << "Test of the function " << __current_test_name << " : ";                                    \
        fflush(stdout);                                                                                          \
        __its_unit_test_cpp_running = true;                                                                      \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                               \
        auto start = std::chrono::steady_clock::now();                                                           \
        func();                                                                                                  \
        auto end = std::chrono::steady_clock::now();                                                             \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); \
        __its_unit_test_cpp_running = false;                                                                     \
        __its_unit_test_cpp_load.join();                                                                         \
        std::cout << COLOR_GREEN << "Success" << COLOR_RESET << " ("                                             \
                  << (double)elapsed_time / 1000 << "ms)" << std::endl;                                          \
        fflush(stdout);                                                                                          \
    } while (0)

/** 
 * Macro that est if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expr)                             \
    do                                           \
    {                                            \
        if (!(expr))                             \
        {                                        \
            __its_unit_test_cpp_running = false; \
            __its_unit_test_cpp_load.join();     \
            test_error(#expr);                   \
            exit(1);                             \
        }                                        \
    } while (0)

/**
 * Macro to make a timed test
 * @param func: the function tested
 * @param timeout_millis: the maximum time of execution in milli-seconds
 */
#define TTEST(func, timeout_millis)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        std::string __current_test_name = #func;                                                                            \
        std::cout << "Test of the function " << __current_test_name << " : ";                                               \
        fflush(stdout);                                                                                                     \
        __its_unit_test_cpp_running= true;                                                                                  \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                                          \
        auto start = std::chrono::steady_clock::now();                                                                      \
        std::thread launch_func = std::thread([]() {func(); __its_unit_test_cpp_running= false; });                         \
        __its_unit_test_cpp_timeout((timeout_millis), launch_func, __its_unit_test_cpp_load);                               \
        auto end = std::chrono::steady_clock::now();                                                                        \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();            \
        std::cout << COLOR_GREEN << "Success " << COLOR_RESET << " (" << (double)elapsed_time / 1000 << "ms)" << std::endl; \
        fflush(stdout);                                                                                                     \
    } while (0)

/* A boolean that indicate if the test is running */
static bool __its_unit_test_cpp_running;
/* The thread that display the loading effect */
static std::thread __its_unit_test_cpp_load;

/**
 * While the test is running, create a waiting animation
 */
static void __its_unit_test_cpp_loadingEffect()
{
    int ind = 0;
    auto print_time = std::chrono::steady_clock::now();

    while (__its_unit_test_cpp_running)
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - print_time).count() > 300)
        {
            switch (ind)
            {
                case 0:
                    std::cout << "   \b\b\b";
                    break;
                case 1:
                    std::cout << ".  \b\b\b";
                    break;
                case 2:
                    std::cout << ".. \b\b\b";
                    break;
                case 3:
                    std::cout << "...\b\b\b";
                    break;
            }
            fflush(stdout);
            ind = (ind+1)%4;
            print_time = std::chrono::steady_clock::now();
        }
    }
    std::cout << "   \b\b\b";
    fflush(stdout);
}

/**
 * Stop the programm if the test is not finished after a certain time
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 * @param load: the thread that display the waiting animation
 */
static void __its_unit_test_cpp_timeout(float time, std::thread &launch_func, std::thread &load)
{
    auto start_time = std::chrono::steady_clock::now();
    while (__its_unit_test_cpp_running)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() > time)
        {
            __its_unit_test_cpp_running= false;
            std::cout << COLOR_RED << "Timeout (" << time << " ms)" << COLOR_RESET  << std::endl;
            fflush(stdout);
            launch_func.detach();
            load.join();
            exit(1);
        }
    launch_func.join();
    load.join();
}

#endif
