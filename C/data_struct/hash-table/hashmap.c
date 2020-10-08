#include "hashmap.h"

int
comp (void *a, void *b)
{
	return *(int *)a == *(int *)b;
}

hash_table_t *create_table(unsigned long long size,
	                       unsigned long long (*hash_function)
						   (hash_table_t *, void *),
					       int (*comp_function)(void *, void *))
{
    hash_table_t *h_map = malloc(sizeof(hash_table_t));

	if(!h_map)
		return NULL;

    h_map->size = size;
    h_map->list = calloc(size, sizeof(hash_node_t *));

	if(h_map->list == NULL)
	{
		free(h_map);
		return NULL;
	}

	h_map->hash_function = hash_function ? hash_function : hash_code;
	h_map->comp_function = comp_function ? comp_function : comp;
    return h_map;
}

unsigned long long hash_code(hash_table_t *h_map, void *key)
{
    return (unsigned long long)key % h_map->size;
}

int insert(hash_table_t *h_map, void *key, void *val)
{
    hash_node_t *list, *temp, *new_node;

    unsigned long long pos = h_map->hash_function(h_map,key);
    if (pos > h_map->size)
        return;

    list = h_map->list[pos];
    temp = list;

    while(temp){
        if(h_map->comp_function(temp->key, key) == 0){
            free(temp->val);
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    new_node = malloc(sizeof(hash_node_t));

	if(new_node == NULL)
		return -1;

    new_node->val = malloc(sizeof(h_map->val_memsize));
	if(new_node->val == NULL)
	{
		free(new_node);
		return -1;
	}
	memcpy(new_node->val, val, h_map->val_memsize);
    new_node->key = malloc(sizeof(h_map->key_memsize));
	if(new_node->key == NULL)
	{
		free(new_node->val);
		free(new_node);
		return -1;
	}
	memcpy(new_node->key, key, h_map->key_memsize);
    new_node->next = list;
    h_map->list[pos] = new_node;
	return 0;
}

void* lookup(hash_table_t *h_map, void *key)
{
    unsigned long long pos   = h_map->hash_function(h_map, key);
    hash_node_t        *list = h_map->list[pos];
    hash_node_t        *temp = list;
    while(temp){
        if(h_map->comp_function(temp->key, key) == 0)
            return temp->val;
        temp = temp->next;
    }
    return NULL;
}

void free_hash_map(hash_table_t *h_map)
{
    for (unsigned long long i = 0; i < h_map->size; i++)
        if(h_map->list[i] != NULL)
        {
            free(h_map->list[i]->val);
            free(h_map->list[i]->key);
            free(h_map->list[i]);
        }
    free(h_map->list);
    free(h_map);
}
