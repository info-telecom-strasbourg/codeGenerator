#include "hashmap.h"

hash_table_ts *
create_table(unsigned long long size,
             size_t key_memsize,
             size_t val_memsize,
             unsigned long long (*hash_function)(hash_table_ts *, void *),
             int (*comp_function)(void *, void *))
{
    hash_table_ts *h_map = malloc(sizeof(hash_table_ts));

    if (!h_map)
        return NULL;

    h_map->size = size;
    h_map->key_memsize = key_memsize;
    h_map->val_memsize = val_memsize;
    h_map->list = calloc(size, sizeof(hash_node_ts *));

    if (h_map->list == NULL)
    {
        free(h_map);
        return NULL;
    }

    h_map->hash_function = hash_function ? hash_function : __hash_code;
    h_map->comp_function = comp_function ? comp_function : __comp;
    return h_map;
}

int insert(hash_table_ts *h_map, void *key, void *val)
{
    hash_node_ts *list, *temp, *new_node;

    unsigned long long pos = h_map->hash_function(h_map, key);
    if (pos > h_map->size)
        return -1;

    list = h_map->list[pos];
    temp = list;

    while (temp)
    {
        if (h_map->comp_function(temp->key, key))
        {
            memcpy(temp->val, val, h_map->val_memsize);
            return 0;
        }
        temp = temp->next;
    }

    new_node = malloc(sizeof(hash_node_ts));
    if (new_node == NULL)
        return -1;

    new_node->val = malloc(h_map->val_memsize);
    if (new_node->val == NULL)
    {
        free(new_node);
        return -1;
    }

    new_node->key = malloc(h_map->key_memsize);
    if (new_node->key == NULL)
    {
        free(new_node->val);
        free(new_node);
        return -1;
    }

    memcpy(new_node->val, val, h_map->val_memsize);
    memcpy(new_node->key, key, h_map->key_memsize);
    new_node->next = list;
    h_map->list[pos] = new_node;
    return 0;
}

int lookup(hash_table_ts *h_map, void *key, void *value)
{
    unsigned long long pos = h_map->hash_function(h_map, key);
    hash_node_ts *list = h_map->list[pos];
    hash_node_ts *temp = list;
    while (temp)
    {
        if (h_map->comp_function(temp->key, key))
        {
            memcpy(value, temp->val, h_map->val_memsize);
            return 0;
        }
        temp = temp->next;
    }
    return -1;
}

void delete_node(hash_table_ts *h_map, void *key)
{
    unsigned long long pos = h_map->hash_function(h_map, key);
    hash_node_ts *temp = h_map->list[pos];
    hash_node_ts *prec = NULL;

    while (temp)
    {
        if (h_map->comp_function(temp->key, key))
        {
            if (prec != NULL)
                prec->next = temp->next;
            else
                h_map->list[pos] = NULL;

            free(temp->val);
            free(temp->key);
            free(temp);
            return;
        }
        prec = temp;
        temp = temp->next;
    }
}

void delete_hash_map(hash_table_ts *h_map)
{
    for (unsigned long long i = 0; i < h_map->size; i++)
        if (h_map->list[i] != NULL)
        {
            hash_node_ts *temp = h_map->list[i];
            while (temp)
            {
                hash_node_ts *next = temp->next;
                free(temp->val);
                free(temp->key);
                free(temp);
                temp = next;
            }
        }
    free(h_map->list);
    free(h_map);
}

unsigned long long
__hash_code(hash_table_ts *h_map, void *key)
{
    return (*(unsigned long long *)key) % h_map->size;
}

int __comp(void *a, void *b)
{
    return *(char *)a == *(char *)b;
}
