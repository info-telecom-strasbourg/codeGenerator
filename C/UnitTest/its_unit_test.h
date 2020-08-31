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
 * @brief Macro to allows to redefine macros with a different parameters number
 */
#define __ITS_GET_MACRO_TEST(_1, _2, NAME, ...) NAME

/**
 * @brief Macro to execute a test without a check of the output
 *
 * To make this library easy to use, this macro can be used with 1
 * (if no timeout is required) or 2 (if a timeout is required)
 * parameters.
 *
 * @param function: the tested function
 * @param timeout_millis: the maximum time of execution in
 * milli-seconds (optional)
 */
#define TEST(...)                                                 \
    __ITS_GET_MACRO_TEST(__VA_ARGS__, __ITS_TEST_2, __ITS_TEST_1) \
    (__VA_ARGS__)

/**
 * @brief Macro to allows to redefine macros with a different parameters
 * number.
 */
#define __ITS_GET_MACRO_OTEST(_1, _2, _3, NAME, ...) NAME

/**
 * @brief Macro to execute a test with a check of the output
 *
 * To make this library easy to use, this macro can be used with 2
 * (if no timeout is required) or 3 (if a timeout is required)
 * parameters.
 *
 * @param function: the tested function
 * @param expected_output_file : the expected output file
 * @param timeout_millis: the maximum time of execution in milli-seconds
 * (optional)
 */
#define OTEST(...)                                                 \
    __ITS_GET_MACRO_OTEST(__VA_ARGS__, __ITS_TEST_4, __ITS_TEST_3) \
    (__VA_ARGS__)

/**
 * @brief Macro to execute a test without specific features
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 */
#define __ITS_TEST_1(function) do{test1(#function, function);}while (0)

/**
 * @brief Macro to execute a test with a timeout
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param timeout_millis: the time given to the test
 */
#define __ITS_TEST_2(function, timeout_millis)\
    do{test2(#function, function, timeout_millis);}while(0)

/**
 * @brief Macro to execute a test of the output of the function
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param expected_output_file: the file that will be compared with the
 * output.
 */
#define __ITS_TEST_3(function, expected_output_file)\
    do{test3(#function, function, expected_output_file);}while(0)

/**
 * @brief Macro to execute a test of the output of the function
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param expected_output_file: the file that will be compared with the
 * output.
 */
#define __ITS_TEST_4(function, expected_output_file, timeout_millis)\
    do{\
        test4(#function, function, expected_output_file, timeout_millis);\
       }while(0)

/**
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expression) do{__its_assert(#expression, expression); } while(0)

/**
 * Macro that test if two files identical
 * @param first_file: the expected file
 * @param second_file : the tested file
 */
#define assert_file(first_file, second_file)\
    do { __its_assert_file(first_file, second_file);} while (0)

/** A global variable indicating if a test is running */
int __its_unit_test_c_running;

/** A global variable where stdout is saved */
int __its_unit_test_save_out;

/** A global variable where stderr is saved */
int __its_unit_test_save_err;

/** A global variable to store the thread used to display a loading effect */
pthread_t __its_unit_test_c_load;

/** A global variable that store the thread used to launch the test function */
pthread_t __its_unit_test_c_func;

/**
 * A global variable that indicates if a timeout is set
 * It's used to stop properly the thread of function.
 */
int __with_timeout;

/**
 * @brief This function testing a function without specific features.
 *
 * The output is redirected to keep a nice effect for the tests.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 */
void test1(char *test_name, void (*function)(void));

/**
 * @brief This function testing a function with a timeout.
 * The output is redirected to keep a nice effect for the tests.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 * @param timeout: the time given to the function before failure.
 */
void test2(char *test_name, void (*function)(void), unsigned long timeout);

/**
 * @brief This function testing a function and it's output.
 *
 * The output is redirected to keep a nice effect for the tests.
 * A file is created to compare with the given file. It is deleted
 * if the test succeeded. If not, it is not deleted.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 * @param expected_output_file: the path to the compare file.
 */
void test3(char *test_name, void (*function)(void),
        char *expected_output_file);

/**
 * @brief This function testing a function and it's output with a timeout.
 *
 * The output is redirected to keep a nice effect for the tests.
 * A file is created to compare with the given file. It is deleted
 * if the test succeeded. If not, it is not deleted.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 * @param expected_output_file: the path to the compare file.
 * @param timeout: the time given to the function before failure.
 */
void test4(char *test_name, void (*function)(void),
        char *expected_output_file, unsigned long timeout_millis);

/**
 * @brief Test the given expression. If it's false the programme is exited.
 *
 * If the test fail, all thread are stoped properly.
 *
 * @param expression_text: the expression tested in string.
 * @param expression: the expression as an integer.
 */
void __its_assert(char* expression_text ,int expression);

/**
 * @brief Test the files given. If they are different, the test fail.
 *
 * If the test fail, all thread are stoped properly.
 *
 * @param expression_text: the expression tested in string.
 * @param expression: the expression as an integer.
 */
void __its_assert_file(char *first_file, char *second_file);
#endif
