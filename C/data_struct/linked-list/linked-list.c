#include "linked-list.h"

list_t *list_create(void)
{
    list_t *l = malloc(sizeof(list_t));
    if(l == NULL)
    {
        return NULL;
    }
    l->beginning_list = NULL;
    l -> size_list = 0;
    return l;
}

void list_delete(list_t *list)
{

}

int list_is_empty(list_t *list)
{

}

int list_append(list_t *list, void *ele)
{
    list_node_t *new_node = malloc(sizeof(list_node_t));
    if(new_node == NULL)
    {
        return 0;
    }

    new_node->data = *ele;
    (list->size_list)++;
    new_node->next_node = NULL;

    if(l->begin == NULL)
    {
        l->begin = new_node;
        return 1;
    }
    list_node_t *last_node = l->begin;
    while(last_node->next != NULL)
    {
        last_node = last_node->next;
    }
    last_node->next = new_node;
    return 1; 
}

int list_prepend(list_t *list, void *ele)
{

}

int list_pop(list_t *list, void *ele)
{

}

int list_size(list_t *list)
{
    return list -> size_list;
}