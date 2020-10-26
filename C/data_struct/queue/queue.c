#include "queue.h"

queue_s *
create_queue(size_t node_memsize)
{
    if (node_memsize == 0)
        return NULL;
    queue_s *queue = malloc(sizeof(queue_s));
    if(queue == NULL)
        return NULL;
    queue->head = queue->tail = NULL;
    queue->node_memsize = node_memsize;
    return queue;
}

int
enqueue(queue_s *queue, const void *data)
{
    queue_node_s *new_node = (queue_node_s *)malloc(sizeof(queue_node_s));

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
dequeue(queue_s *queue, void *data)
{
    if (queue->head == NULL)
        return -1;

    queue_node_s *temp = queue->head;
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
queue_peek(const queue_s *queue, void *data)
{
    if (queue->head == NULL)
        return -1;

    queue_node_s *temp = queue->head;
    memcpy(data, temp->data, queue->node_memsize);
    return 0;
}

void
delete_queue(queue_s *queue)
{
    queue_node_s *temp;

    while (queue->head != NULL)
    {
        temp = queue->head;
        queue->head = queue->head->next;
        free(temp->data);
        free(temp);
    }

    free(queue);
}

int
queue_is_empty(const queue_s *queue)
{
    return queue->head == NULL;
}
