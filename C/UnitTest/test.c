#include "unit_test.h"

#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <unistd.h>

void test(void)
{
    sleep(2);
    fprintf(stdout, "This is a message\n");
    fprintf(stderr, "This is an error message\n");
    assert(2==2);
}

int main(void)
{
    OTEST(test, "output_1.txt", 1000);
}