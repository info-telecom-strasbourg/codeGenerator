#include "hashmap.h"


hash_table_t *create_table(unsigned long long size)
{
    hash_table_t *h_map = malloc(sizeof(hash_table_t));
    h_map->size = size;
    h_map->list = malloc(sizeof(hash_node_t*) * size);
    for (unsigned long long i = 0; i < size; i++)
        h_map->list[i] = NULL;
    return h_map;
}

unsigned long long hash_code(hash_table_t *h_map, void *key)
{
    return (unsigned long long)key % h_map->size;
}

void insert(hash_table_t *h_map, void *key, void *val)
{
    hash_node_t *list, *temp, *new_node;

    unsigned long long pos = hash_code(h_map,key);
    if (pos > h_map->size) {
        return;
    }

    list = h_map->list[pos];
    temp = list;

    while(temp){
        if(temp->key == key){
            free(temp->val);
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    new_node = malloc(sizeof(hash_node_t));
    new_node->key = key;
    new_node->val = val;
    new_node->next = list;
    h_map->list[pos] = new_node;

}

void* lookup(hash_table_t *h_map, void *key)
{
    unsigned long long pos   = hash_code(h_map, key);
    hash_node_t        *list = h_map->list[pos];
    hash_node_t        *temp = list;
    while(temp){
        if(temp->key == key){
            return temp->val;
        }
        temp = temp->next;
    }
    return NULL;
}

void free_hash_map(hash_table_t *h_map)
{
    for (unsigned long long i = 0; i < h_map->size; i++) {
        if(h_map->list[i] != NULL)
        {
            free(h_map->list[i]->val);
            free(h_map->list[i]);
        }
    }
    free(h_map->list);
    free(h_map);
}
