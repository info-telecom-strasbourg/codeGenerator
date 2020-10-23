#include "queue.h"


int
create_queue(queue_t *queue, size_t node_memsize)
{
    if (node_memsize == 0)
        return -1;
    queue->head = queue->tail = NULL;
    queue->node_memsize = node_memsize;
    return 0;
}

int
enqueue(queue_t *queue, const void *data)
{
    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));

    if (queue == NULL || new_node == NULL)
        return -1;

    new_node->data = malloc(queue->node_memsize);

    if (new_node->data == NULL)
    {
        free(new_node);
        return -1;
    }

    new_node->next = NULL;

    memcpy(new_node->data, data, queue->node_memsize);

    if (queue->head == NULL)
        queue->head = queue->tail = new_node;
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    return 0;
}

int
dequeue(queue_t *queue, void *data)
{
    if (queue->head == NULL)
        return -1;

    queue_node_t *temp = queue->head;
    memcpy(data, temp->data, queue->node_memsize);

    if(queue->tail != queue->head)
        queue->head = queue->head->next;
    else
        queue->head = queue->tail = NULL;

    free(temp->data);
    free(temp);
    return 0;
}

int
queue_peek(const queue_t *queue, void *data)
{
    if (queue->head == NULL)
        return -1;

    queue_node_t *temp = queue->head;
    memcpy(data, temp->data, queue->node_memsize);
    return 0;
}

void
delete_queue(queue_t *queue)
{
    queue_node_t *temp;

    while (queue->head != NULL)
    {
        temp = queue->head;
        queue->head = queue->head->next;
        free(temp->data);
        free(temp);
    }

    queue->head = queue->tail = NULL;
}

int
queue_is_empty(const queue_t *queue)
{
    return queue->head == NULL;
}
