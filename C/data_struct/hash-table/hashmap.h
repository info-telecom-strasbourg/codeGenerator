#ifndef HASH_TABLE_HASHMAP_H
#define HASH_TABLE_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
