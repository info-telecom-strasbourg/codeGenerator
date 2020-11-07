#ifndef HASH_TABLE_HASHMAP_H
#define HASH_TABLE_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************** How to use a HashMap *****************************
* A HashMap is a data strucutre. It contains Data-Key pairs.                   *
* The keys in the HashMap are unique. Different keys may have the same value,  *
* thus the values may not be unique.                                           *
* One could insert the <K, V> pair in a list but then one would need to scan   *
* the entire list to find the value associated withe the key. With a HasMap    *
* one wants to reduce the search area. Thus, the long list of items is break   *
* into n small lists as shown below: a HashMap ist a table of lists.           *
* Whenever a key is given, one search for the key in those small lists.        *
* If the key is found, the value is returned. Otherwise there is no <K, V>     *
* Otherwise there is no <K, V> pair for the given key.                         *
* The size of the HashMap is equal to the number of small lists.               *
*                                                                              *
* To look up a value the key is given as input to the hash function and returns*
* the slot number or the list number in which to search the key. Then the      *
* value is searched in the list. If the key is found in the list, the          *
* corresponding value is returned.                                             *
*                                                                              *
*        ____________                                                          *
*       |            |                                                         *
*  _____|__       ___v____       ________       ________       ________        *
* | slot 1 |     | K1, V1 | ->  | K2, V2 | ->  | K3, V3 | ... | Kn, Vn |       *
*  --------       --------       --------       --------       --------        *
*                                                                              *
*        ____________                                                          *
*       |            |                                                         *
*  _____|__       ___v____       ________       ________       ________        *
* | slot 2 |     | Ka, Va | ->  | Kb, Vb | ->  | Kc, Vc | ... | Kz, Vz |       *
*  --------       --------       --------       --------       --------        *
*                                                                              *
*   .                                                                          *
*   .                                                                          *
*   .                                                                          *
*                                                                              *
*        ____________                                                          *
*       |            |                                                         *
*  _____|__       ___v______      __________      __________       __________  *
* | slot n |     | Ka1, Va1 | -> | Kb2, Vb2 | -> | Kc3, Vc3 | ... | Kzn, Vzn | *
*  --------       ----------      -----------     ----------       ----------  *
*                                                                              *
*                                                                              *
*******************************************************************************/

/**
 * @brief A structure representing a node in the hash table.
 *
 * A node store a data (any type) and points to the next node in the list.
 */
typedef struct node
{
    void               *key;    /**< The key of the node.
                                  It can be anything. */
    void               *val;    /**< The data stored in the node.
                                  It can be anything. */
    struct node        *next;   /**< Pointer to identify the next node. */
} hash_node_s;

/**
 * @brief A structure representing a hash table.
 * 
 * A hash table is a data structure that link a key and a value.
 * To use it correctly, please use insert and lookup to store and
 * find a value. You can have a personnal hash and comparaison function .
 */
typedef struct table
{
    unsigned long long size;           /**< The size of the subsequent list. */
	size_t             val_memsize;    /**< Memory size of a value */
	size_t             key_memsize;    /**< Memory size of a value */
	unsigned long long (*hash_function)(struct table *, void *); /**< The hash
																    function. */
	int (*comp_function)(void *, void *); /**< The comparaison function, it must
											   return 1 if the two arguments
											   are equals and 0 if they are
											   not. */
    hash_node_s        **list;          /**< Pointer to identify the table of
											 subsequent lists. */
} hash_table_s;

/**
 * @brief Macro to allows to redefine macros with a different parameters number
 */
#define __ITS_GET_MACRO_CREATE_TABLE(_1, _2, _3, _4, _5, NAME, ...) NAME

/**
 * @brief Macro to create a hash map
 *
 * To make this library easy to use, this macro can be used with 5 or 3
 * parameters.
 *
 * @param size: the size of the hash map.
 * @param key_memsize: the memsize of a key.
 * @param val_memsize: the memsize of a value.
 * @param hash_func: the hash function.
 * @param comp_func: the comparaison function.
 */
#define create_table(...)                                                           \
	__ITS_GET_MACRO_CREATE_TABLE(__VA_ARGS__, __CREAT_T5, __CREAT_T4, __CREAT_T3) \
	(__VA_ARGS__)

#define __CREAT_T5(size, key_memsize, val_memsize, hash_func, comp_func) create_table(size, key_memsize, val_memsize, hash_func, comp_func)

#define __CREAT_T3(size, key_memsize, val_memsize) create_table(size, key_memsize, val_memsize, NULL, NULL)

/**
 * @brief Creates a HashMap with the given size.
 * 
 * It allocate memory for the table and for the list of nodes. It also
 * initialize the hash and compare functions. If it's NULL, default
 * functions will be used (the functions __comp and __hash_code).
 *
 * @param size: size of the HashMap (number of node).
 * @param key_memsize: the size of the keys you will use in your table.
 * @param val_memsize: the size of the values you will store in your table.
 * @param hash_function: the hash function that will be used (if NULL is given,
 * __hash_code will be used by default).
 * @param comp_function: thecompar function that will be used to compare keys
 * (if NULL is given, __comp will be used).
 * @return the pointer to the created HashMap.
 */
hash_table_s *create_table(unsigned long long size,
	size_t key_memsize,
	size_t val_memsize,
	unsigned long long (*hash_function)(hash_table_s *, void *),
	int (*comp_function)(void *, void *));

/**
 * @brief Insert a <K, V> pair in the HashMap.
 *
 * It allocates memory for the node, the key and the value. If an allocation
 * fail, it clean the memory allocated by the function call.
 * 
 * @param h_map: the pointer to the HashMap.
 * @param key: the key of the pair.
 * @param val: the value of the pair.
 * @return a number that indicate if the <K, V> was correctly inserted (0 for
 * success -1 for error).
 */
int insert(hash_table_s *h_map, void *key, void *val);

/**
 * @brief Search the key in the HashMap and returns the corresponding value.
 *
 * Please allocate memory for the value.
 * ___Do not___
 * int* data;
 * lookup(my_h_map, my_key, data);
 * ___Do____
 * int data;
 * lookup(my_h_map, my_key, &data);
 * 
 * @param h_map: the pointer to the HashMap
 * @param key: the key of the pair
 * @param val: the pointer to store the data.
 * @return an integer that indicate if the function succeeded (0 for success
 * -1 for error)
 */
int lookup(hash_table_s *h_map, void *key, void *value);

/**
 * @brief Free the memory allocated by a specific node.
 *
 * @param h_map: a pointer to the Hashmap.
 * @param key: the key associated to the node you want to free.
 */
void delete_node(hash_table_s *h_map, void* key);

/**
 * @brief Delete a HashMap and frees all the memory allocated by it.
 *
 * @param h_map: the pointer to the HashMap
 */
void delete_hash_map(hash_table_s *h_map);

/**
 * @brief Get the slot number in which to search for the key.
 * 
 * This is the default hash function. It will be replaced if you give a
 * function at create_table.
 *
 * @param h_map: the pointer to the HashMap.
 * @param key: the key of the pair.
 * @return the slot or list number in which to search for the key.
 */
unsigned long long __hash_code(hash_table_s *h_map, void *key);

/**
 * @brief Compare two values.
 * 
 * It is the default function. If you give a function at create_table, it will
 * be replaced.
 * 
 * @param a: the adress of the first value to compare.
 * @param b: the adress of the second value to compare.
 * @return an int that indicate if the two values are equals (0 if they are
 * different and 1 if they are equals).
 */
int __comp (void *a, void *b);

#endif //HASH_TABLE_HASHMAP_H
