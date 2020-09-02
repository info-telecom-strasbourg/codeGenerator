/**
 * Template file to understand the usage of the library "unit_test.hpp" for C++
 */

#include "its_unit_test.h" //include it to have an access to the library

#include <iostream>
#include <unistd.h>
#include <vector>

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
    std::string string_1 = "hello";
    std::string string_2 = "hello";

    assert(string_1 == string_2);
}

/**
 * Check that two vectors are identical
 */
void test_compare_vectors(void)
{
    std::vector<int> array_1(10000);
    std::vector<int> array_2(10000);

    for (int i(0); i < 10000; ++i)
    {
        array_1[i] = i;
        array_2[i] = i;
    }

    assert(array_1 == array_2);
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
    std::cout << "This is a message" << std::endl;
    std::cout << "This is an error message" << std::endl;
}

/**
 * A function that display messages on the terminal and lasts 2 seconds
 */
void test_output_and_timeout(void)
{
    std::cout << "This is a message" << std::endl;
    std::cout << "This is an error message" << std::endl;
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
    std::cout << "--- Launch tests ---" << std::endl << std::endl;

    TEST(test_equality);
    TEST(test_compare_string);
    TEST(test_compare_vectors);
    TEST(test_timeout, 2100);
    OTEST(test_output, "output_1.txt");
    OTEST(test_output, "output_1.txt", 2100);
    TEST(test_file_comparison);
    TEST(test_fail);

    return EXIT_SUCCESS;
}
