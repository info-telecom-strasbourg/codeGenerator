# hash_table for C

## Content

This library allows you to use hash tables in C. It defines several types such as `hash_table_s` and `hash_node_s`. You can use both of them but the second one is already used by the library so you won't have to use it. Don't bother about the content of those 2 structures, you won't have to use them. Everything is managed by the library. You just have to use `hash_table_s`.

## Example of utilisation

```c
int main(void)
{
	//Créer la hash table
    hash_table_s *my_hash_map = create_table(5, sizeof(unsigned long long), sizeof(int), NULL, NULL);
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

//Exemple with functions and structures
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
