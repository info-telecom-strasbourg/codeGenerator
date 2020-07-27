#ifndef ITS_UNIT_TEST_H
#define ITS_UNIT_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>

/* Macros for colors */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"


/**
 * Macro to execute a test
 * @param func: the tested function 
 */
#define TEST(func)                                                                                                                                                     \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        char *__current_test_name = #func;                                                                                                                             \
        fprintf(stdout, "Test of the function %s : ", __current_test_name);                                                                                            \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                            \
        if (pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL) == -1)                                                            \
        {                                                                                                                                                              \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        func();                                                                                                                                                        \
        gettimeofday(&end, NULL);                                                                                                                                      \
        __its_unit_test_c_running = false;                                                                                                                           \
        if (pthread_join(__its_unit_test_c_load, NULL))                                                                                                              \
        {                                                                                                                                                              \
            perror("Loading effect thread join failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
    } while (0)

/**
 * Macro to make a timed test
 * @param func: the function tested
 * @param timeout_millis: the maximum time of execution in milli-seconds
 */
#define TTEST(func, timeout_millis)                                                                                                                                    \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        char *__current_test_name = #func;                                                                                                                             \
        fprintf(stdout, "Test of the function %s : ", __current_test_name);                                                                                            \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                            \
        if (pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL) == -1)                                                            \
        {                                                                                                                                                              \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        pthread_t launch_func;                                                                                                                                         \
        if (pthread_create(                                                                                                                                            \
                &launch_func, NULL, ({void *callback() {func(); __its_unit_test_c_running= false;} callback; }), NULL) == -1)                                                                                                                \
        {                                                                                                                                                              \
            perror("Function launcher creation failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        __its_unit_test_c_timeout((timeout_millis), &launch_func);                                                                                                   \
        gettimeofday(&end, NULL);                                                                                                                                      \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
    } while (0)

/** 
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expr)                                                                              \
    do                                                                                            \
    {                                                                                             \
        if (!(expr))                                                                              \
        {                                                                                         \
            __its_unit_test_c_running = false;                                                  \
            if (pthread_join(__its_unit_test_c_load, NULL))                                     \
            {                                                                                     \
                perror("Loading effect thread join failed");                                      \
                exit(EXIT_FAILURE);                                                               \
            }                                                                                     \
            fprintf(stdout, "%sFailed%s\nassertion failed : %s\n", COLOR_RED, COLOR_RESET, #expr); \
            fflush(stdout);                                                                       \
            exit(1);                                                                              \
        }                                                                                         \
    } while (0)

/* A boolean that indicate if the test is running */
static bool __its_unit_test_c_running;
/* The thread that display the loading effect */
static pthread_t __its_unit_test_c_load;

/**
 * While the test is running, create a waiting animation
 */
static void *__its_unit_test_c_loadingEffect(void *arg)
{
    int ind = 0;
    struct timeval print_time, cur_time;
    gettimeofday(&print_time, NULL);

    while (__its_unit_test_c_running)
    {
        gettimeofday(&cur_time, NULL);
        if (((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (print_time.tv_sec * 1000000 + print_time.tv_usec)) / 1000 > 300)
        {
            switch (ind)
            {
            case 0:
                fprintf(stdout, "   \b\b\b");
                break;
            case 1:
                fprintf(stdout, ".  \b\b\b");
                break;
            case 2:
                fprintf(stdout, ".. \b\b\b");
                break;
            case 3:
                fprintf(stdout, "...\b\b\b");
                break;
            }
            fflush(stdout);
            ind = (ind + 1) % 4;
            gettimeofday(&print_time, NULL);
        }
    }
    fprintf(stdout, "   \b\b\b");
    fflush(stdout);
}

/**
 * Stop the programm if the test is not finished after a certain time
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void __its_unit_test_c_timeout(long int time, pthread_t *launch_func)
{
    struct timeval start_time, cur_time;
    gettimeofday(&start_time, NULL);
    while (__its_unit_test_c_running)
    {
        gettimeofday(&cur_time, NULL);
        if (((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)) / 1000 > time)
        {
            __its_unit_test_c_running = false;
            fprintf(stdout, "%sTimeout (%ld ms)%s\n", COLOR_RED, time, COLOR_RESET);
            fflush(stdout);
            if (pthread_detach(*launch_func))
            {
                perror("Function launcher thread join failed");
                exit(EXIT_FAILURE);
            }
            if (pthread_join(__its_unit_test_c_load, NULL))
            {
                perror("Loading effect thread join failed");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
    }
    if (pthread_join(*launch_func, NULL))
    {
        perror("Function launcher thread join failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(__its_unit_test_c_load, NULL))
    {
        perror("Loading effect thread join failed");
        exit(EXIT_FAILURE);
    }
}

#endif
