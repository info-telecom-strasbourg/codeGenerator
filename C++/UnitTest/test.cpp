#include "unit_test.hpp"

#include <iostream>
#include "unistd.h"
#include <vector>

void test_equality(void)
{

    assert(1 == 1);

    std::cerr << "This text won't be displayed!" << std::endl;

    sleep(2);
}

void test_string(void)
{
    std::string string_1 = "hello";
    std::string string_2 = "hello";

    std::cout << "This text won't be displayed!" << std::endl;

    assert(string_1 == string_2);
}

void test_compare_vectors_success(void)
{
    std::vector<int> array_1(100000000);
    std::vector<int> array_2(100000000);

    for (int i(0); i < 100000000; ++i)
    {
        array_1[i] = i;
        array_2[i] = i;
    }

    std::cout << "This text won't be displayed!" << std::endl;

    assert(array_1 == array_2);
    // assert_file("test_1.txt", "test_2.txt");
}

void test_compare_vectors_fail(void)
{

    std::cout << "This text won't be displayed!" << std::endl;
    std::vector<int> array_1(100000000);
    std::vector<int> array_2(100000000);

    for (int i(0); i < 100000000; ++i)
    {
        array_1[i] = i;
        array_2[i] = i + 1;
    }


    assert(array_1 == array_2);
    assert_file("test_1.txt", "test_2.txt");
}

int main(void)
{
    TEST(test_compare_vectors_success);
    TEST(test_compare_vectors_success);
    TEST(test_compare_vectors_success);
  

    return EXIT_SUCCESS;
}