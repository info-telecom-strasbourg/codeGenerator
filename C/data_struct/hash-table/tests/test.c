#include "its_unit_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

long long __remaining_primsys_its = -1;
long long __remaining_alloc_its = -1;
long long __remaining_threads_fct_its = -1;

struct test_struct {
	int data_int;
	char data_char[3];
};

struct test_keys {
	int key_int;
	char key_str[3];
};

unsigned long long test_hash_func(hash_table_s *h_map, void *key)
{
	struct test_keys *key2 = (struct test_keys *)key;
	return (key2->key_int + key2->key_str[0] + key2->key_str[1])%h_map->size;
}

int test_comp_func(void *a, void *b)
{
	struct test_keys *a2 = (struct test_keys *)a;
	struct test_keys *b2 = (struct test_keys *)b;
	return a2->key_int == b2->key_int &&
	       a2->key_str[0] == b2->key_str[0] &&
		   a2->key_str[1] == b2->key_str[1] &&
		   a2->key_str[2] == b2->key_str[2] &&
		   a2->key_str[3] == '\0';
}

void
test_create_table(void)
{
    hash_table_s *hash_map = create_table(5, sizeof(int), sizeof(int), NULL, NULL);
    assert(hash_map != NULL);
    assert(hash_map->size == 5);
    assert(hash_map->val_memsize == sizeof(int));
    assert(hash_map->key_memsize == sizeof(int));
    assert(hash_map->hash_function == hash_code);
    assert(hash_map->comp_function == comp);
    assert(hash_map->list != NULL);
    free_hash_map(hash_map);

    hash_table_s *hash_map2 = create_table(5, sizeof(struct test_keys), sizeof(int), test_hash_func, test_comp_func);
    assert(hash_map2 != NULL);
    assert(hash_map2->size == 5);
    assert(hash_map2->val_memsize == sizeof(int));
    assert(hash_map2->key_memsize == sizeof(struct test_keys));
    assert(hash_map2->hash_function == test_hash_func);
    assert(hash_map2->comp_function == test_comp_func);
    assert(hash_map2->list != NULL);
    free_hash_map(hash_map2);
}

void
test_hash_code(void)
{
	hash_table_s *hash_map = create_table(5, sizeof(unsigned long long), sizeof(int), NULL, NULL);
	unsigned long long key_1 = 4;
	unsigned long long key_2 = 8;
	unsigned long long key_3 = 9;
	assert((hash_code(hash_map, &key_1)) == (unsigned long long)4);
	assert(hash_code(hash_map, &key_2) == 3);
	assert(hash_code(hash_map, &key_3) == 4);
	free_hash_map(hash_map);
}

void
test_comp(void)
{
	unsigned long long a = 4;
	unsigned long long b = 4;
	assert(comp(&a,&b));
	b = 5;
	assert(!comp(&a,&b));
}

void
test_insert(void)
{
	hash_table_s *hash_map = create_table(5, sizeof(unsigned long long), sizeof(int), NULL, NULL);
	unsigned long long keys[5] = {0, 1, 2, 3, 4};
	int vals[5] = {10, 20, 30, 40, 50};
	int i;
	for(i = 0; i < 5; i++)
		insert(hash_map, &(keys[i]), &(vals[i]));

	for(i = 0; i < 5; i++)
	{
		assert(*(int *)hash_map->list[i]->val == vals[i]);
		assert(*(unsigned long long *)hash_map->list[i]->key == keys[i]);
	}

	unsigned long long keys_2[5] = {5, 6, 7, 8, 9};
	int vals_2[5] = {50, 60, 70, 80, 90};
	for (i = 0; i < 5; i++)
		insert(hash_map, &(keys_2[i]), &(vals_2[i]));

	for(i = 0; i < 5; i++)
	{
		assert(*(int *)hash_map->list[i]->next->val == vals[i]);
		assert(*(int *)hash_map->list[i]->val == vals_2[i]);
		assert(*(unsigned long long *)hash_map->list[i]->next->key == keys[i]);
		assert(*(unsigned long long *)hash_map->list[i]->key == keys_2[i]);
	}
	free_hash_map(hash_map);

	hash_table_s *hash_map2 = create_table(7, sizeof(struct test_keys), sizeof(struct test_struct), test_hash_func, test_comp_func);
	struct test_keys keys2[3] = {{1, "un"}, {2, "de"}, {3, "tr"}};
	struct test_struct vals2[3] = {{10, "di"}, {20, "vi"}, {30, "tr"}};
	for(i = 0; i < 5; i++)
		insert(hash_map2, &(keys2[i]), &(vals2[i]));

	assert(*(int *)hash_map2->list[(keys2[0].key_int + keys2[0].key_str[0] + keys2[0].key_str[1])%hash_map2->size]->val == vals2[0].data_int);
	assert(*(int *)hash_map2->list[(keys2[1].key_int + keys2[1].key_str[0] + keys2[1].key_str[1])%hash_map2->size]->next->val == vals2[1].data_int);
	assert(*(int *)hash_map2->list[(keys2[2].key_int + keys2[2].key_str[0] + keys2[2].key_str[1])%hash_map2->size]->val == vals2[2].data_int);
	// faire avec les arrays
	free_hash_map(hash_map2);
}

void
test_lookup(void)
{

}

void
test_free_hash_map(void)
{

}

void
test_delete_node(void)
{

}

int
main(void)
{
    TEST(test_create_table);
    TEST(test_hash_code);
    TEST(test_comp);
    TEST(test_insert);
    TEST(test_lookup);
    TEST(test_free_hash_map);
    TEST(test_delete_node);
}
