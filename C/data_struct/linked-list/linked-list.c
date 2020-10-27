#include <stdlib.h>
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
    list_node_t *last_node = list -> beginning_list;
    list_node_t *last_last_node = NULL;

    if (last_node == NULL)
    {
        free(list);
    }
    else
    {
        while (last_node -> next_node != NULL)
        {   
            free(last_last_node);
            last_last_node = last_node;
            last_node = last_node->next_node;
        }
        free(list);
        free(last_last_node);
        free(last_node);
    }
}

int list_is_empty(list_t *list)
{
    return (list->size_list == 0);
}

int list_append(list_t *list, void *ele)
{
    list_node_t *new_node = malloc(sizeof(list_node_t));
    if(new_node == NULL)
    {
        return 0;
    }
    new_node->data = ele;
    list->size_list++;
    new_node->next_node = NULL;

    if(list->beginning_list == NULL)
    {
        list->beginning_list = new_node;
        return 1;
    }
    else
    {
        list_node_t *last_node = list->beginning_list;
        while(last_node->next_node != NULL)
        {
            last_node = last_node->next_node;
        }
        last_node->next_node = new_node;
        return 1;
    }
    
    
}

int list_prepend(list_t *list, void *ele)
{
    list_node_t *first_node = malloc(sizeof(list_node_t));
        if(first_node == NULL)
        {
            return 0;
        }
    if (list -> beginning_list == NULL) //the linked list is empty
    {
        first_node->data = ele;
        first_node->next_node = NULL;
        list->beginning_list = first_node;
        list->size_list++;
    }
    else
    {
        list_node_t *adr_first_node = list -> beginning_list;
        first_node->data = ele;
        list->beginning_list = first_node;
        first_node->next_node = adr_first_node;
        list->size_list++;
    }
    return 0;
}

int list_pop(list_t *list, void *ele)
{
    list_node_t *last_node = list->beginning_list;
    list_node_t *last_last_node = NULL;

    if (last_node == NULL) //the linked list is empty
    {
        return 1;
    }
    else
    {
        last_node = last_node->next_node;
        while(last_node != NULL)
        {
            last_last_node = last_node;
            last_node = last_node->next_node;
        }

        ele = last_node->data;
        last_last_node->next_node = NULL;

        return 0;
    }
}

int list_size(list_t *list)
{
    return list -> size_list;
}