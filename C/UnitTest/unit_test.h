#ifndef ITS_UNIT_TEST_H
#define ITS_UNIT_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* Macros for colors */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"

/**
 * Macro to allows to redefine macros with a different parameters number
 */
#define __ITS_GET_MACRO_TEST(_1, _2, NAME, ...) NAME

/**
 * Macro to execute a test without a check of the output
 * @param function: the tested function 
 * @param timeout_millis: the maximum time of execution in milli-seconds (optionnal)
 */
#define TEST(...)                                                 \
    __ITS_GET_MACRO_TEST(__VA_ARGS__, __ITS_TEST_2, __ITS_TEST_1) \
    (__VA_ARGS__)

/**
 * Macro to allows to redefine macros with a different parameters number
 */
#define __ITS_GET_MACRO_OTEST(_1, _2, _3, NAME, ...) NAME

/**
 * Macro to execute a test with a check of the output
 * @param function: the tested function 
 * @param expected_output_file : the expected output file
 * @param timeout_millis: the maximum time of execution in milli-seconds (optionnal)
 */
#define OTEST(...)                                                 \
    __ITS_GET_MACRO_OTEST(__VA_ARGS__, __ITS_TEST_4, __ITS_TEST_3) \
    (__VA_ARGS__)

/**
 * Macro to execute a test without a check of the output
 * @param function: the tested function 
 */
#define __ITS_TEST_1(function)                                                                                                                                         \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        save_out = dup(fileno(stdout));                                                                                                                                \
        char *__current_test_name = #function;                                                                                                                         \
        fprintf(stdout, "Check %s : ", __current_test_name);                                                                                                           \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                              \
        if ((errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL)) != 0)                                                       \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        int out = open("/dev/null", O_RDWR | O_APPEND);                                                                                                          \
        if (out == -1)                                                                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Open of \"/dev/null\" failed");                                                                                                                    \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        int save_err = dup(fileno(stderr));                                                                                                                            \
        if (dup2(out, fileno(stdout)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stdout redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        if (dup2(out, fileno(stderr)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stderr redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        function();                                                                                                                                                    \
        gettimeofday(&end, NULL);                                                                                                                                      \
        __its_unit_test_c_running = false;                                                                                                                             \
        if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread join failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        fflush(stdout);                                                                                                                                                \
        fflush(stderr);                                                                                                                                                \
        close(out);                                                                                                                                                    \
        dup2(save_out, fileno(stdout));                                                                                                                                \
        dup2(save_err, fileno(stderr));                                                                                                                                \
        close(save_out);                                                                                                                                               \
        close(save_err);                                                                                                                                               \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
    } while (0)

int save_out;
/**
 * Macro to make a timed test
 * @param func: the function tested
 * @param timeout_millis: the maximum time of execution in milli-seconds
 */
#define __ITS_TEST_2(function, timeout_millis)                                                                                                                         \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        save_out = dup(fileno(stdout));                                                                                                                                \
        char *__current_test_name = #function;                                                                                                                         \
        fprintf(stdout, "Check %s : ", __current_test_name);                                                                                                           \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                              \
        if ((errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL)) != 0)                                                       \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        int out = open("/dev/null", O_RDWR | O_APPEND);                                                                                                          \
        if (out == -1)                                                                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Open of \"/dev/null\" failed");                                                                                                                    \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        int save_err = dup(fileno(stderr));                                                                                                                            \
        if (dup2(out, fileno(stdout)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stdout redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        if (dup2(out, fileno(stderr)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stderr redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        pthread_t launch_func;                                                                                                                                         \
        if ((errno = pthread_create(                                                                                                                                   \
                 &launch_func, NULL, ({void *callback() {function(); __its_unit_test_c_running= false;} callback; }), NULL)) != 0)                                                                                                               \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Function launcher creation failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        __its_unit_test_c_timeout((timeout_millis), &launch_func);                                                                                                     \
        gettimeofday(&end, NULL);                                                                                                                                      \
        __its_unit_test_c_running = false;                                                                                                                             \
        if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread join failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        fflush(stdout);                                                                                                                                                \
        fflush(stderr);                                                                                                                                                \
        close(out);                                                                                                                                                    \
        dup2(save_out, fileno(stdout));                                                                                                                                \
        dup2(save_err, fileno(stderr));                                                                                                                                \
        close(save_out);                                                                                                                                               \
        close(save_err);                                                                                                                                               \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
    } while (0)

#define __ITS_TEST_3(function, expected_output_file)                                                                                                                   \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        save_out = dup(fileno(stdout));                                                                                                                                \
        char *__current_test_name = #function;                                                                                                                         \
        fprintf(stdout, "Check %s : ", __current_test_name);                                                                                                           \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                              \
        if ((errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL)) != 0)                                                       \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 0666);                                                                                                  \
        if (out == -1)                                                                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Open of \"_its_out_test.log\" failed");                                                                                                            \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        int save_err = dup(fileno(stderr));                                                                                                                            \
        if (dup2(out, fileno(stdout)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stdout redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        if (dup2(out, fileno(stderr)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stderr redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        function();                                                                                                                                                    \
        gettimeofday(&end, NULL);                                                                                                                                      \
        assert_file(expected_output_file, "_its_out_test.log");                                                                                                            \
        __its_unit_test_c_running = false;                                                                                                                             \
        if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread join failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        fflush(stdout);                                                                                                                                                \
        fflush(stderr);                                                                                                                                                \
        close(out);                                                                                                                                                    \
        dup2(save_out, fileno(stdout));                                                                                                                                \
        dup2(save_err, fileno(stderr));                                                                                                                                \
        close(save_out);                                                                                                                                               \
        close(save_err);                                                                                                                                               \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
        remove("_its_out_test.log");                                                                                                                                       \
    } while (0)

#define __ITS_TEST_4(function, expected_output_file, timeout_millis)                                                                                                   \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        save_out = dup(fileno(stdout));                                                                                                                                \
        char *__current_test_name = #function;                                                                                                                         \
        fprintf(stdout, "Check %s : ", __current_test_name);                                                                                                           \
        fflush(stdout);                                                                                                                                                \
        __its_unit_test_c_running = true;                                                                                                                              \
        if ((errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL)) != 0)                                                       \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread creation failed");                                                                                                           \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        int out = open("_its_out_test.log", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0666);                                                                                                  \
        if (out == -1)                                                                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Open of \"_its_out_test.log\" failed");                                                                                                            \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        int save_err = dup(fileno(stderr));                                                                                                                            \
        if (dup2(out, fileno(stdout)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stdout redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        if (dup2(out, fileno(stderr)) == -1)                                                                                                                           \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("stderr redirection failed");                                                                                                                       \
            return EXIT_FAILURE;                                                                                                                                       \
        }                                                                                                                                                              \
        struct timeval start, end;                                                                                                                                     \
        gettimeofday(&start, NULL);                                                                                                                                    \
        pthread_t launch_func;                                                                                                                                         \
        if ((errno = pthread_create(                                                                                                                                   \
                 &launch_func, NULL, ({void *callback() {function(); __its_unit_test_c_running= false;} callback; }), NULL)) != 0)                                                                                                               \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Function launcher creation failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        __its_unit_test_c_timeout((timeout_millis), &launch_func);                                                                                                     \
        gettimeofday(&end, NULL);                                                                                                                                      \
        assert_file(expected_output_file, "_its_out_test.log");                                                                                                            \
        __its_unit_test_c_running = false;                                                                                                                             \
        if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                                                                                 \
        {                                                                                                                                                              \
            dup2(save_out, fileno(stderr));                                                                                                                            \
            perror("Loading effect thread join failed");                                                                                                               \
            exit(EXIT_FAILURE);                                                                                                                                        \
        }                                                                                                                                                              \
        fflush(stdout);                                                                                                                                                \
        fflush(stderr);                                                                                                                                                \
        close(out);                                                                                                                                                    \
        dup2(save_out, fileno(stdout));                                                                                                                                \
        dup2(save_err, fileno(stderr));                                                                                                                                \
        close(save_out);                                                                                                                                               \
        close(save_err);                                                                                                                                               \
        fprintf(stdout, "%sSuccess%s (%ld ms)\n", COLOR_GREEN, COLOR_RESET, ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000); \
        fflush(stdout);                                                                                                                                                \
        remove("_its_out_test.log");                                                                                                                                       \
    } while (0)

/**
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expression)                                                                                 \
    do                                                                                                     \
    {                                                                                                      \
        if (!(expression))                                                                                 \
        {                                                                                                  \
            __its_unit_test_c_running = false;                                                             \
            if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                 \
            {                                                                                              \
                dup2(save_out, fileno(stderr));                                                            \
                perror("Loading effect thread join failed");                                               \
                exit(EXIT_FAILURE);                                                                        \
            }                                                                                              \
            dprintf(save_out, "%sFailed%s\nassertion failed : %s\n", COLOR_RED, COLOR_RESET, #expression); \
            exit(EXIT_FAILURE);                                                                            \
        }                                                                                                  \
    } while (0)

/**
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expression)                                                                                 \
    do                                                                                                     \
    {                                                                                                      \
        if (!(expression))                                                                                 \
        {                                                                                                  \
            __its_unit_test_c_running = false;                                                             \
            if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                 \
            {                                                                                              \
                dup2(save_out, fileno(stderr));                                                            \
                perror("Loading effect thread join failed");                                               \
                exit(EXIT_FAILURE);                                                                        \
            }                                                                                              \
            dprintf(save_out, "%sFailed%s\nassertion failed : %s\n", COLOR_RED, COLOR_RESET, #expression); \
            exit(EXIT_FAILURE);                                                                            \
        }                                                                                                  \
    } while (0)

// fscanf(message, "%sFailed%s\nassertion failed : %s\n", COLOR_RED, COLOR_RESET, #expression); \


#define assert_file(file1, file2)                                                                       \
    do                                                                                                  \
    {                                                                                                   \
        int file_origi = open(file1, O_RDONLY);                                                         \
        int file_saved = open(file2, O_RDONLY);                                                         \
        if (file_origi == -1 || file_saved == -1)                                                       \
        {                                                                                               \
            dup2(save_out, fileno(stderr));                                                             \
            perror("A problem occured when files were opened\n");                                       \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        char buf_origi[1024] = "", buf_saved[1024] = "";                                                \
        int read_origi = 0, read_saved = 0, to_compare = 0;                                             \
        while ((read_origi = read(file_origi, buf_origi, 1024)) >= 0 &&                                 \
               (read_saved = read(file_saved, buf_saved, 1024)) > 0)                                    \
            if (read_origi != read_saved || memcmp(buf_origi, buf_saved, read_saved))                   \
            {                                                                                           \
                dprintf(save_out, "%sFailed%s\nThe two files are different\n", COLOR_RED, COLOR_RESET); \
                exit(EXIT_FAILURE);                                                                     \
            }                                                                                           \
        if (read_origi == -1 || read_saved == -1)                                                       \
        {                                                                                               \
            dup2(save_out, fileno(stderr));                                                             \
            perror("An error occured while reading files\n");                                           \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        close(file_saved);                                                                              \
        close(file_origi);                                                                              \
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
                dprintf(save_out, "   \b\b\b");
                break;
            case 1:
                dprintf(save_out, ".  \b\b\b");
                break;
            case 2:
                dprintf(save_out, ".. \b\b\b");
                break;
            case 3:
                dprintf(save_out, "...\b\b\b");
                break;
            }
            ind = (ind + 1) % 4;
            gettimeofday(&print_time, NULL);
        }
    }
    dprintf(save_out, "   \b\b\b");
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
            dprintf(save_out, "%sTimeout (%ld ms)%s\n", COLOR_RED, time, COLOR_RESET);
            if ((errno = pthread_detach(*launch_func)) != 0)
            {
                dup2(save_out, fileno(stderr));
                perror("Function launcher thread join failed");
                exit(EXIT_FAILURE);
            }
            if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)
            {
                dup2(save_out, fileno(stderr));
                perror("Loading effect thread join failed");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
    }
    if ((errno = pthread_join(*launch_func, NULL)) != 0)
    {
        dup2(save_out, fileno(stderr));
        perror("Function launcher thread join failed");
        exit(EXIT_FAILURE);
    }
}

#endif