# hash_table for C

## Content

This library allows you to use hash tables in C. It defines several types such as `hash_table_ts` and `hash_node_ts`. You can use both of them but the second one is already used by the library so you won't have to use it. Don't bother about the content of those 2 structures, you won't have to use them. Everything is managed by the library. You just have to use `hash_table_ts`.

## Example of utilisation

```c
int main(void)
{
	//Créer la hash table
    hash_table_ts *my_hash_map = create_table(5, sizeof(unsigned long long), sizeof(int), NULL, NULL);
	unsigned long long key1 = 1;
	int val1 = 3;

	// stock la valeur associé à la clé donnée
	insert(hash_map, &(key1), &(val1));

	unsigned long long key2 = 2;
	int val2 = 4;

	// stock la valeur associé à la clé donnée
	insert(hash_map, &(key2), &(val2));

	//Récupère la valeur associée à la clé donnée
	int val_recup;
	lookup(my_hash_map, &(key1), &val_recup);

	//Supprime la valeur associé à la clé donnée
	delete_node(my_hash_map, &(key1));

	//Libère la mémoire allouée par la hash table
	delete_hash_map(my_hash_map);
}
__________________________________________________
//Exemple with functions and structures

struct test {
	int data_int;
	char data_str[5];
};

int 
my_comp (void *a, void *b)
{
	struct test a1 = *(struct test *)a;
	struct test b1 = *(struct test *)b;
	return a1.data_int == b.data_int && 
		   strcmp(a1.data_str, b1.data_str) == 0;
}

unsigned long long 
my_hash_code(hash_table_ts *h_map, void *key)
{
	int key_int = *(int *)key;
	return (key_int*5)%h_map->size; 
}

int main(void)
{
	//Créer la hash table
    hash_table_ts *my_hash_map = create_table(5, sizeof(unsigned long long), sizeof(struct test), my_hash_code, my_comp);
	unsigned long long key1 = 1;
	struct test val1 = {3, "truc"};

	// stock la valeur associé à la clé donnée
	insert(hash_map, &(key1), &(val1));

	unsigned long long key2 = 2;
	int val2 = {4, "autr"};

	// stock la valeur associé à la clé donnée
	insert(hash_map, &(key2), &(val2));

	//Récupère la valeur associée à la clé donnée
	struct test val_recup;
	lookup(my_hash_map, &(key1), &val_recup);

	//Supprime la valeur associé à la clé donnée
	delete_node(my_hash_map, &(key1));

	//Libère la mémoire allouée par la hash table
	delete_hash_map(my_hash_map);
}
```

If you want to have an overview of how to use this library, you can take a look at the *Example* folder.

**Warning** Do not allocate memory for the data you add to the sack. The library manage the memory it use. If you want to use strings or array, just gives the size of the arrays or strings in `create_table`.

## Advices

Do not forget to call `delete_hash_map` when you do not use the hash table anymore, it will free the memory allocated by the library.

If you use arrays or strings, you should use fixed memory size (`char[10]` instead of `char *` or `int[3]` instead of `int *`). If you decide to use non fixed arrays, you will have to manage the memory yourself because the library do not manage allocated variables.

## Warnings

The size given to `create_table` is essential. It defines what you want to store in the queue.

## Example

You can run the example by using `make` to compile and `./test` to run the tests or `make test` to compile and run the tests.


## Contributors
- [Clara SCHILD](https://github.com/cschild)
- [Thomas RIVES](https://github.com/ThomasRives)
