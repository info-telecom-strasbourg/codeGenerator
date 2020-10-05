#include "hashmap.h"


hash_table_t *createTable(unsigned long long size)
{
    hash_table_t *t = malloc(sizeof(hash_table_t));
    t->size = size;
    t->list = malloc(sizeof(hash_node_t*) * size);
    for (unsigned long long i = 0; i < size; i++)
        t->list[i] = NULL;
    t->capacite = 0;
    return t;
}

unsigned long long hashCode(hash_table_t *t, unsigned long long key)
{
    return key%t->size;
}

void insert(hash_table_t *t, unsigned long long key, void *val)
{
    unsigned long long pos = hashCode(t,key);
    if (pos > t->size) {
        return;
    }
    hash_node_t *list = t->list[pos];
    hash_node_t *temp = list;

    if(temp == NULL)
    {
        t->capacite++;
    }

    while(temp){
        if(temp->key==key){
            free(temp->val);
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    hash_node_t *newNode = malloc(sizeof(hash_node_t));
    newNode->key = key;
    newNode->val = val;
    newNode->countVal = 0;
    newNode->next = list;
    t->list[pos] = newNode;

}

void* lookup(hash_table_t *t, unsigned long long key)
{
    unsigned long long pos   = hashCode(t,key);
    hash_node_t        *list = t->list[pos];
    hash_node_t        *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return NULL;
}

hash_node_t** lookupNode(hash_table_t *t, unsigned long long key)
{
    unsigned long long pos   = hashCode(t,key);
    hash_node_t        *list = t->list[pos];
    hash_node_t        *temp = list;
    while(temp){
        if(temp->key==key){
            return &(t->list[pos]);
        }
        temp = temp->next;
    }
    return NULL;
}

void freeHashMap(hash_table_t *hMap)
{
    for (unsigned long long i = 0; i < hMap->size; i++) {
        if(hMap->list[i] != NULL)
        {
            free(hMap->list[i]->val);
            free(hMap->list[i]);
        }
    }
    free(hMap->list);
    free(hMap);
}
