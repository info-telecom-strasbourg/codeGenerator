/**
 * Library of unit test for C++
 * Developped by Hugo LAULLIER and Thomas RIVES
 */

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
 #define __ITS_TEST_1(function)\
     do{test1(#function, function);}while (0)

#define __ITS_TEST_2(function, timeout_millis)\
    do{test2(#function, function, timeout_millis);}while(0)

#define __ITS_TEST_3(function, expected_output_file)\
    do{test3(#function, function, expected_output_file);}while(0)

#define __ITS_TEST_4(function, expected_output_file, timeout_millis)\
    do{test4(#function, function, expected_output_file, timeout_millis);}while(0)

/**
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expression)                                                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        if (!(expression))                                                                                                  \
        {                                                                                                                   \
            __its_unit_test_c_running = 0;                                                                              \
            if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                                  \
            {                                                                                                               \
                dup2(__its_unit_test_save_err, fileno(stderr));                                                             \
                perror("Loading effect thread join failed");                                                                \
                exit(EXIT_FAILURE);                                                                                         \
            }                                                                                                               \
            dprintf(__its_unit_test_save_out, "%sFailed%s\nassertion failed : %s\n", "\x1b[1;31m", "\x1b[0m", #expression); \
            exit(EXIT_FAILURE);                                                                                             \
        }                                                                                                                   \
    } while (0)

/**
 * Macro that test if two files identical
 * @param first_file: the expected file
 * @param second_file : the tested file
 */
#define assert_file(first_file, second_file)                                                                             \
    do                                                                                                                   \
    {                                                                                                                    \
        int file_origi = open(first_file, O_RDONLY);                                                                     \
        int file_saved = open(second_file, O_RDONLY);                                                                    \
        if (file_origi == -1 || file_saved == -1)                                                                        \
        {                                                                                                                \
            dup2(__its_unit_test_save_err, fileno(stderr));                                                              \
            perror("A problem occured when files were opened\n");                                                        \
            exit(EXIT_FAILURE);                                                                                          \
        }                                                                                                                \
        char buf_origi[1024] = "", buf_saved[1024] = "";                                                                 \
        int read_origi = 0, read_saved = 0, to_compare = 0;                                                              \
        while ((read_origi = read(file_origi, buf_origi, 1024)) >= 0 &&                                                  \
               (read_saved = read(file_saved, buf_saved, 1024)) > 0)                                                     \
            if (read_origi != read_saved || memcmp(buf_origi, buf_saved, read_saved))                                    \
            {                                                                                                            \
                if ((errno = pthread_join(__its_unit_test_c_load, NULL)) != 0)                                           \
                {                                                                                                        \
                    dup2(__its_unit_test_save_err, fileno(stderr));                                                      \
                    perror("Loading effect thread join failed");                                                         \
                    exit(EXIT_FAILURE);                                                                                  \
                }                                                                                                        \
                dprintf(__its_unit_test_save_out, "%sFailed%s\nThe two files are different\n", "\x1b[1;31m", "\x1b[0m"); \
                exit(EXIT_FAILURE);                                                                                      \
            }                                                                                                            \
        if (read_origi == -1 || read_saved == -1)                                                                        \
        {                                                                                                                \
            dup2(__its_unit_test_save_err, fileno(stderr));                                                              \
            perror("An error occured while reading files\n");                                                            \
            exit(EXIT_FAILURE);                                                                                          \
        }                                                                                                                \
        if (close(file_saved) == -1 || close(file_origi) == -1)                                                          \
        {                                                                                                                \
            perror("Close of a file failed");                                                                            \
            exit(EXIT_FAILURE);                                                                                          \
        }                                                                                                                \
    } while (0)


int __its_unit_test_c_running;
int __its_unit_test_save_out;
int __its_unit_test_save_err;
pthread_t __its_unit_test_c_load;
void test1(char *test_name, void (*function)(void));
void test2(char *test_name, void (*function)(void), unsigned long timeout);
void test3(char *test_name, void (*function)(void), char *expected_output_file);
void test4(char *test_name, void (*function)(void), char *expected_output_file, unsigned long timeout_millis);
#endif
