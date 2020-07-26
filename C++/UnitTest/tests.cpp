#include <iostream>
#include <vector>

#include "unit_test.hpp"

void test_1 (void)
{
    assert(1 == 1);
}

void test_2 (void)
{
    assert(1 == 2);
}

void test_3 (void)
{
    assert(2 == 2);
}

int main (void)
{
    TEST(test_1);
    TEST(test_2);
    TEST(test_3);

    
    return EXIT_SUCCESS;
}
