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
 * You can have a personnal hash and comparaison function .
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
 * @brief Creates a HashMap with the given size.
 * 
 * It allocate memory for the table and for the list of nodes. It also
 * initialize the hash and compare functions. If it's NULL, default
 * functions will be used (the functions __comp and __hash_code).
 *
 * @param size size of the HashMap (number of node).
 * @param hash_function the hash function that will be used (if NULL is given,
 * __hash_code will be used by default).
 * @param comp_function thecompar function that will be used to compare keys
 * (if NULL is given, __comp will be used).
 *
 * @return the pointer to the created HashMap.
 */
 hash_table_s *create_table(unsigned long long size,
	                        size_t key_memsize,
	                        size_t val_memsize,
 	                        unsigned long long (*hash_function)
 						    (hash_table_s *, void *),
 					        int (*comp_function)(void *, void *));
/**
 * @brief Get the slot number in which to search for the key.
 *
 * @param h_map the pointer to the HashMap.
 * @param key the key of the pair.
 *
 * @return the slot or list number in which to search for the key
 */
unsigned long long __hash_code(hash_table_s *h_map, void *key);

/**
 * @brief Insert a <K, V> pair in the HashMap.
 *
 * It allocates memory for the node, the key and the value. If an allocation
 * fail, it clean the memory allocated by the function call.
 * 
 * @param h_map the pointer to the HashMap.
 * @param key the key of the pair.
 * @param val the value of the pair.
 *
 * @return a number that indicate if the <K, V> was correctly inserted (0 for
 * success -1 for error)
 */
int insert(hash_table_s *h_map, void *key, void *val);

/**
 * @brief Search the key in the HashMap and returns the corresponding value.
 *
 * @param h_map the pointer to the HashMap
 * @param key the key of the pair
 * @param val the pointer to store the data
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
 * @param h_map the pointer to the HashMap
 */
void delete_hash_map(hash_table_s *h_map);

int __comp (void *a, void *b);

#endif //HASH_TABLE_HASHMAP_H
