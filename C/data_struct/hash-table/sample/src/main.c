#include <stdio.h>
#include "hashmap.h"

int main(void) {
	hash_table_s *table = create_table(2, sizeof(int), sizeof(int), NULL, NULL);
	int key = 1;
	int j = 1;
	int value;
	insert(table, &key, &j);
	lookup(table, &key, &value);
	printf("voici la key %d et la val %d\n", key, value);
	delete_hash_map(table);
	return 0;
}
