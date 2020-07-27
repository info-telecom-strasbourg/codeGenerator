#include <iostream>
#include <vector>

#include "unit_test.hpp"

void test_1 (void)
{
    sleep(2);
    assert(2 == 2);
}

void test_2 (void)
{
    sleep(2);
    assert(2 == 2);
}

void test_3 (void)
{
    for (int i = 0; i < 530000000; i++)
        assert(2 == 2);
}

int main (void)
{
    TEST(test_1);
    TEST(test_2);
    TTEST(test_3, 500);
        
    return EXIT_SUCCESS;
}
