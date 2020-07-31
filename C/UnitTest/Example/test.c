/**
 * Template file to understand the usage of the library "unit_test.h" for C
 */

#include "../unit_test.h" //include it to have an access to the library

#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

/**
 * Test an equality
 */
void test_equality(void)
{
    assert(1 == 1);
}

/**
 * Check that two strings are identical
 */
void test_compare_string(void)
{
    char string_1[5] = "hello";
    char string_2[5] = "hello";

    assert(strcmp(string_1, string_1) == 0);
}

/**
 * A function that lasts 2 seconds
 */
void test_timeout(void)
{
    sleep(2);
}

/**
 * A function that display messages on the terminal
 */
void test_output(void)
{
    fprintf(stdout, "This is a message\n");
    fprintf(stderr, "This is an error message\n");
}

/**
 * A function that display messages on the terminal and lasts 2 seconds
 */
void test_output_and_timeout(void)
{
    fprintf(stdout, "This is a message\n");
    fprintf(stderr, "This is an error message\n");
    sleep(2);
}

/**
 * Check that two files are identical
 */
void test_file_comparison(void)
{
    assert_file("output_1.txt", "output_2.txt");
}

/**
 * Test a wrong equality (this test have to fail)
 */
void test_fail(void)
{
    assert(1 != 1);
}

int main(void)
{
    TEST(test_equality);
    TEST(test_compare_string);
    TEST(test_timeout, 2100);
    OTEST(test_output, "output_1.txt");
    OTEST(test_output_and_timeout, "output_1.txt", 2100);
    TEST(test_file_comparison);
    TEST(test_fail);

    return EXIT_SUCCESS;
}
