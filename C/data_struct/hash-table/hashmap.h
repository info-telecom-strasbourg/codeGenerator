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
* into n small lists as shown below. Whenever a key is given, one search for   *
* the key in those small lists. If the key is found, the value is returned.    *
* Otherwise there is no <K, V> pair for the given key.                         *
* The size of the HashMap is equal to the number of small lists.

* To look up a value the key is given as input to the hash function and returns*
* the slot number in which to search the value. Then the value is searched in  *
* the list. If the key is found in the list, the corresponding value is        *
* returned.
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


typedef struct node
{
    void               *key;
	unsigned long long index;
    void               *val;
    struct node        *next;
} hash_node_t;

typedef struct table
{
    unsigned long long size;
	size_t             node_memsize;
    hash_node_t        **list;
} hash_table_t;

/**
 * Cree une hashMap
 */
hash_table_t *create_table(unsigned long long size, size_t node_memsize);

/**
 * Recupere la position dans la hashmap à partir de la clé
 */
unsigned long long hash_code_int(hash_table_t *t, unsigned long long key);

/**
 * Insère une valeur dans la hashmap pour une clé donnée
 */
void insert(hash_table_t *t, unsigned long long key, void *val);

/**
 * Recherche la valeur associée à une clé donnée
 */
void* lookup(hash_table_t *t, unsigned long long key);

/**
 * Recherche l'adresse du noeud associée à une clé donnée
 * un noeud est un elément de la hashmap (une hashmap est un tableau de noeuds)
 */
hash_node_t** lookup_node(hash_table_t *t, unsigned long long key);

/**
 * Libère la hashmap
 */
void free_hash_map(hash_table_t *hMap);


#endif //HASH_TABLE_HASHMAP_H
