/**
 * Library of unit test for C++
 * Developped by Hugo LAULLIER and Thomas RIVES
 */

#ifndef ITS_UNIT_TEST_H
#define ITS_UNIT_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*******************************************************************************
*                                   Tests                                      *
*******************************************************************************/

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
#define TEST(...)                                                              \
    __ITS_GET_MACRO_TEST(__VA_ARGS__, __ITS_TEST_2, __ITS_TEST_1)              \
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
#define OTEST(...)                                                             \
    __ITS_GET_MACRO_OTEST(__VA_ARGS__, __ITS_TEST_4, __ITS_TEST_3)             \
    (__VA_ARGS__)

/**
 * @brief Macro to execute a test without specific features
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 */
#define __ITS_TEST_1(function)                                                 \
	do{__test_classic_unittest_its(#function, function);}while (0)

/**
 * @brief Macro to execute a test with a timeout
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param timeout_millis: the time given to the test
 */
#define __ITS_TEST_2(function, timeout_millis)                                 \
    do{                                                                        \
		__test_timeout_unittest_its(#function, function, timeout_millis);      \
	}while(0)

/**
 * @brief Macro to execute a test of the output of the function
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param expected_output_file: the file that will be compared with the
 * output.
 */
#define __ITS_TEST_3(function, expected_output_file)                           \
    do{                                                                        \
		__test_output_unittest_its(#function, function, expected_output_file); \
	}while(0)

/**
 * @brief Macro to execute a test of the output of the function
 *
 * The standard outputs are redirected to keep a nice output.
 *
 * @param function: the tested function
 * @param expected_output_file: the file that will be compared with the
 * output.
 */
#define __ITS_TEST_4(function, expected_output_file, timeout_millis)           \
    do{                                                                        \
        __test_output_timeout_unittest_its(#function, function,                \
			expected_output_file, timeout_millis);                             \
       }while(0)

/**
 * Macro that test if the the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expression)                                                     \
	do{__assert_unittest_its(#expression, expression); } while(0)

/**
 * Macro that test if two files identical
 * @param first_file: the expected file
 * @param second_file : the tested file
 */
#define assert_file(first_file, second_file)                                   \
    do { __assert_file_unittest_its(first_file, second_file);} while (0)

/*******************************************************************************
*                            Memory allocation                                 *
*******************************************************************************/

/**
 * The number of memory allocation that will succeed.
 * If it is negative, they will all succeed.
 */
extern long long __remaining_alloc_its;

/**
 * @brief Redefine calloc to make it fail when you want.
 *
 * calloc can still fail. NULL may be return even if __remaining_alloc_its != 0
 *
 * @param a: the first parameter of calloc.
 * @param b: the second parameter of calloc.
 * @return the result of classic calloc or NULL if commanded failure
 */
#define calloc(a,b)                                                            \
	((__remaining_alloc_its > 0)                                               \
		? __remaining_alloc_its--, calloc(a,b)                                 \
		: (__remaining_alloc_its < 0)                                          \
			? calloc(a,b)                                                      \
			: NULL)

/**
 * @brief Redefine malloc to make it fail when you want.
 *
 * malloc can still fail. NULL may be return even if __remaining_alloc_its != 0
 *
 * @param a: the first parameter of realloc.
 * @param b: the second parameter of realloc.
 * @return the result of classic realloc or NULL if commanded failure
 */
#define malloc(a)                                                              \
	((__remaining_alloc_its > 0)                                               \
		? __remaining_alloc_its--, malloc(a)                                   \
		: (__remaining_alloc_its < 0)                                          \
			? malloc(a)                                                        \
			: NULL)

/**
 * @brief Redefine realloc to make it fail when you want.
 *
 * realloc can still fail. NULL may be return even if __remaining_alloc_its != 0
 *
 * @param a: the parameter of malloc.
 * @return the result of classic malloc or NULL if commanded failure
 */
#define realloc(a,b)                                                           \
	((__remaining_alloc_its > 0)                                               \
		? __remaining_alloc_its--, realloc(a,b)                                \
		: (__remaining_alloc_its < 0)                                          \
			? realloc(a,b)                                                     \
			: NULL)

/*******************************************************************************
*                            Primitive System                                  *
*******************************************************************************/

#ifdef PRIM_SYST
extern long long __remaining_primsys_its;

#define access(a,b)                                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, access(a,b)                               \
		: (__remaining_primsys_its < 0)                                        \
			? access(a,b)                                                      \
			: -1)

#define stat(a,b)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, stat(a,b)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? stat(a,b)                                                        \
			: -1)

#define open_macro(_1, _2, _3, NAME, ...) NAME

#define open(...)                                                              \
    open_macro(__VA_ARGS__, open_2, open_1)                                    \
    (__VA_ARGS__)

#define open_1(a,b)                                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, open(a,b)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? open(a,b)                                                        \
			: -1)

#define open_2(a,b,c)                                                          \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, open(a,b,c)                               \
		: (__remaining_primsys_its < 0)                                        \
			? open(a,b,c)                                                      \
			: -1)

#define close(a)                                                               \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, close(a)                                  \
		: (__remaining_primsys_its < 0)                                        \
			? close(a)                                                         \
			: -1)

#define read(a,b,c)                                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, read(a,b,c)                               \
		: (__remaining_primsys_its < 0)                                        \
			? read(a,b,c)                                                      \
			: -1)

#define write(a,b,c)                                                           \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, write(a,b,c)                              \
		: (__remaining_primsys_its < 0)                                        \
			? write(a,b,c)                                                     \
			: -1)

#define lseek(a,b,c)                                                           \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, lseek(a,b,c)                              \
		: (__remaining_primsys_its < 0)                                        \
			? lseek(a,b,c)                                                     \
			: -1)

#define stat(a,b)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, stat(a,b)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? stat(a,b)                                                        \
			: -1)

#define fstat(a,b)                                                             \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, fstat(a,b)                                \
		: (__remaining_primsys_its < 0)                                        \
			? fstat(a,b)                                                       \
			: -1)

#define utime(a,b)                                                             \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, utime(a,b)                                \
		: (__remaining_primsys_its < 0)                                        \
			? utime(a,b)                                                       \
			: -1)

#define chmod(a,b)                                                             \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, chmod(a,b)                                \
		: (__remaining_primsys_its < 0)                                        \
			? chmod(a,b)                                                       \
			: -1)

#define fchmod(a,b)                                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, fchmod(a,b)                               \
		: (__remaining_primsys_its < 0)                                        \
			? fchmod(a,b)                                                      \
			: -1)

#define chown(a,b,c)                                                           \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, chown(a,b,c)                              \
		: (__remaining_primsys_its < 0)                                        \
			? chown(a,b,c)                                                     \
			: -1)

#define fchown(a,b,c)                                                          \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, fchown(a,b,c)                             \
		: (__remaining_primsys_its < 0)                                        \
			? fchown(a,b,c)                                                    \
			: -1)

#define mkdir(a,b)                                                             \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, mkdir(a,b)                                \
		: (__remaining_primsys_its < 0)                                        \
			? mkdir(a,b)                                                       \
			: -1)

#define rmdir(a)                                                               \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, rmdir(a)                                  \
		: (__remaining_primsys_its < 0)                                        \
			? rmdir(a)                                                         \
			: -1)

#define link(a,b)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, link(a,b)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? link(a,b)                                                        \
			: -1)

#define unlink(a)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, unlink(a)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? unlink(a)                                                        \
			: -1)

#define symlink(a,b)                                                           \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, symlink(a,b)                              \
		: (__remaining_primsys_its < 0)                                        \
			? symlink(a,b)                                                     \
			: -1)

#define readlink(a,b,c)                                                        \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, readlink(a,b,c)                           \
		: (__remaining_primsys_its < 0)                                        \
			? readlink(a,b,c)                                                  \
			: -1)

#define lstat(a,b)                                                             \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, lstat(a,b)                                \
		: (__remaining_primsys_its < 0)                                        \
			? lstat(a,b)                                                       \
			: -1)

#define setuid(a)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, setuid(a)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? setuid(a)                                                        \
			: -1)

#define setgid(a)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, setgid(a)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? setgid(a)                                                        \
			: -1)

#define fork()                                                                 \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, fork()                                    \
		: (__remaining_primsys_its < 0)                                        \
			? fork()                                                           \
			: -1)

#define wait(a)                                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, wait(a)                                   \
		: (__remaining_primsys_its < 0)                                        \
			? wait(a)                                                          \
			: -1)

#define waitpid(a,b,c)                                                         \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, waitpid(a,b,c)                            \
		: (__remaining_primsys_its < 0)                                        \
			? waitpid(a,b,c)                                                   \
			: -1)

#define time(a)                                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, time(a)                                   \
		: (__remaining_primsys_its < 0)                                        \
			? time(a)                                                          \
			: -1)

#define stime(a)                                                               \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, stime(a)                                  \
		: (__remaining_primsys_its < 0)                                        \
			? stime(a)                                                         \
			: -1)

#define gettimeofday(a,b)                                                      \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, gettimeofday(a,b)                         \
		: (__remaining_primsys_its < 0)                                        \
			? gettimeofday(a,b)                                                \
			: -1)

#define times(a)                                                               \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, times(a)                                  \
		: (__remaining_primsys_its < 0)                                        \
			? times(a)                                                         \
			: -1)

#define getrusage(a,b)                                                         \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, getrusage(a,b)                            \
		: (__remaining_primsys_its < 0)                                        \
			? getrusage(a,b)                                                   \
			: -1)

#define pipe(a)                                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pipe(a)                                   \
		: (__remaining_primsys_its < 0)                                        \
			? pipe(a)                                                          \
			: -1)

#define mkfifo(a,b)                                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, mkfifo(a,b)                               \
		: (__remaining_primsys_its < 0)                                        \
			? mkfifo(a,b)                                                      \
			: -1)

#define dup(a)                                                                 \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, dup(a)                                    \
		: (__remaining_primsys_its < 0)                                        \
			? dup(a)                                                           \
			: -1)

#define dup2(a,b)                                                              \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, dup2(a,b)                                 \
		: (__remaining_primsys_its < 0)                                        \
			? dup2(a,b)                                                        \
			: -1)
#endif


/*******************************************************************************
*                                 Thread                                       *
*******************************************************************************/

#ifdef THREAD_T
extern long long __remaining_primsys_its;

#define pthread_create(a,b,c,d)                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_create(a,b,c,d)                   \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_create(a,b,c,d)                                          \
			: EAGAIN)

#define pthread_join(a,b)                                                      \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_join(a,b)                         \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_join(a,b)                                                \
			: EAGAIN)

#define pthread_attr_init(a)                                                   \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_attr_init(a)                      \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_attr_init(a)                                             \
			: ENOMEM)

#define pthread_attr_destroy(a)                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_attr_destroy(a)                   \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_attr_destroy(a)                                          \
			: ENOMEM)

#define pthread_kill(a,b)                                                      \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_kill(a,b)                         \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_kill(a,b)                                                \
			: EINVAL)

#define sigwait(a,b)                                                           \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, sigwait(a,b)                              \
		: (__remaining_primsys_its < 0)                                        \
			? sigwait(a,b)                                                     \
			: EINVAL)

#define pthread_barrier_wait(a)                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrier_wait(a)                   \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrier_wait(a)                                          \
			: 2)

#define pthread_barrier_init(a,b,c)                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrier_init(a,b,c)               \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrier_init(a,b,c)                                      \
			: ENOMEN)

#define pthread_barrierattr_init(a)                                            \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrierattr_init(a)               \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrierattr_init(a)                                      \
			: ENOMEN)

#define pthread_barrierattr_destroy(a)                                         \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrierattr_destroy(a)            \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrierattr_destroy(a)                                   \
			: ENOMEN)

#define pthread_barrierattr_setpshared(a,b)                                    \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrierattr_setpshared(a,b)       \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrierattr_setpshared(a,b)                              \
			: EINVAL)

#define pthread_barrierattr_getpshared(a,b)                                    \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_barrierattr_getpshared(a,b)       \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_barrierattr_getpshared(a,b)                              \
			: EINVAL)

#define pthread_mutex_init(a,b)                                                \
	((__remaining_primsys_its > 0)                                             \
		? __remaining_primsys_its--, pthread_mutex_init(a,b)                   \
		: (__remaining_primsys_its < 0)                                        \
			? pthread_mutex_init(a,b)                                          \
			: EINVAL)


#endif

/*******************************************************************************
*                        Functions used in macros                              *
*******************************************************************************/

/**
 * @brief This function testing a function without specific features.
 *
 * The output is redirected to keep a nice effect for the tests.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 */
void __test_classic_unittest_its(char *test_name, void (*function)(void));

/**
 * @brief This function testing a function with a timeout.
 * The output is redirected to keep a nice effect for the tests.
 *
 * @param test_name: the name of the test function.
 * @param function: the function itself.
 * @param timeout: the time given to the function before failure.
 */
void __test_timeout_unittest_its(char *test_name, void (*function)(void),
	unsigned long timeout);

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
void __test_output_unittest_its(char *test_name, void (*function)(void),
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
void __test_output_timeout_unittest_its(char *test_name, void (*function)(void),
        char *expected_output_file, unsigned long timeout_millis);

/**
 * @brief Test the given expression. If it's false the programme is exited.
 *
 * If the test fail, all thread are stoped properly.
 *
 * @param expression_text: the expression tested in string.
 * @param expression: the expression as an integer.
 */
void __assert_unittest_its(char* expression_text ,int expression);

/**
 * @brief Test the files given. If they are different, the test fail.
 *
 * If the test fail, all thread are stoped properly.
 *
 * @param expression_text: the expression tested in string.
 * @param expression: the expression as an integer.
 */
void __assert_file_unittest_its(char *first_file, char *second_file);

#endif
