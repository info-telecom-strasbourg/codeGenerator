#include <stdio.h>
#include "cst.h"
#include "hashmap.h"

int comp_name(void *a, void *b)
{
	return strcmp((char *)a, (char *)b) == 0;
}

unsigned long long hash_func_name(hash_table_ts *table, void *key)
{
	return strlen((char *)key) % table->size;
}

struct gare {
	int horaires[100];
	int id_train[150];
};



int main(void)
{
	hash_table_ts *my_table = create_table(10, sizeof(char) * 50, 
											sizeof(struct gare), 
											hash_func_name,
											comp_name);

	char name[20] = "Gare de l'Est";
	struct gare gare1 = { {1,2,3,4}, {2345, 12345, 987}};
	insert(my_table, name, &gare1);

	char name2[20] = "Gare du Nord";
	struct gare gare2 = {{4,3,2,1}, {987, 3465, 90987, 12345}};
	insert(my_table, name2, &gare2);

	struct gare gare_recup;
	lookup(my_table, name2, &gare_recup);

	printf("first horaire: %d\n", gare_recup.horaires[0]);
	printf("first id: %d\n", gare_recup.id_train[0]);

	struct gare gare_recup2;
	lookup(my_table, name, &gare_recup2);

	printf("first horaire: %d\n", gare_recup2.horaires[0]);
	printf("first id: %d\n", gare_recup2.id_train[0]);

	delete_hash_map(my_table);

	return 0;
}
