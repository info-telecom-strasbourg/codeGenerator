#include "its_unit_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

long long __remaining_primsys_its = -1;
long long __remaining_alloc_its = -1;
long long __remaining_threads_fct_its = -1;

void
test_create_table(void)
{
    hash_table_t *hash_map = create_table(5);
    assert(hash_map != NULL);
    assert(hash_map->size == 5);
    assert(hash_map->list != NULL);
    free_hash_map(hash_map);
}

int
main(void)
{
    TEST(test_create_table);
}