#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <stdlib.h>
#include <string.h>
/***************************** How to use a queue ******************************
* A queue is a data structure. It is a FIFO (First In First Out) list. You     *
* have to use specific methods to access the data stored in the stack. Each    *
* time a data is added, it is stored at the end of the stack. Each time you    *
* access a data, it is the oldest one you stored in the stack.                 *
* Example:                                                                     *
*                __                                                            *
* Initial queue:|  |                                                           *
*               |__|                                                           *
* We add a node:                                                               *
* Queue :   node 1                                                             *
*          ________                                                            *
*         | data 1 |                                                           *
*         |________|                                                           *
* We add another node:                                                         *
* Queue :   node 1   <----   node 2                                            *
*          ________         ________                                           *
*         | data 1 | <---- | data 2 |                                          *
*         |________|       |________|                                          *
* We get a value of the stack:                                                 *
* Queue :   node 1                                                             *
*          ________                                                            *
*         | data 2 |                                                           *
*         |________|                                                           *
* We get the data stored in the first node.                                    *
*******************************************************************************/

/**
 * @brief A structure representing a node in the queue.
 *
 * A node store a data (any type) and points to the next node in the queue.
 */
typedef struct queue_node {
    void              *data; /**< The data stored in the node.
                                  It can be anything. */
    struct queue_node *next; /**< Pointer to identify the next node. */
} queue_node_t;


/**
 * @brief A structure representing a queue.
 *
 * A queue is a succession of node. It must be used as a FIFO
 * (First In First Out) list.
 * To use a queue correctly, please use the functions of this library to
 * enqueue or dequeue data.
 * If you choose to use your own functions please create a function that
 * put data at the end of the queue and a function that remove data at
 * the beginning of the queue.
 */
typedef struct queue {
    size_t       node_memsize; /**< The size of the data that will be stored
                                    in a node. */
    queue_node_t *tail; /**< The tail (last node) of the queue. */
    queue_node_t *head; /**< The head (first node) of the queue. */
} queue_t;

/**
 * @brief Initialize the attributes of a queue.
 *
 * This function check if the node_memsize is not 0 (the size of the data
 * stored can not be  0). Be careful, if you pass a negative parameter, the
 * size_t will convert it to a positive variable and can lead to unexpected
 * behaviour. You may use sizeof(data_type) to have the correct size.
 *
 * @param queue: the queue you want to initialize.
 * @param node_memsize: the size of the data that will be stored in the queue.
 * @return an int that indicate if the creation were done without error:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int create_queue(queue_t *queue, size_t node_memsize);

/**
 * @brief Push a new node at the end of the queue.
 *
 * This function check if memory allocation succeeded. If not, the memory
 * allocated by the function will be freed. The queue will not be freed, just
 * the memory allocated by this call of the function.
 *
 * @param queue: the queue that will receive the new node.
 * @param data: the data that will be stored in the queue.
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int enqueue(queue_t *queue, const void *data);

/**
 * @brief Remove the head of the queue and store the data at the given address.
 *
 * The function free the memory allocated when the node was put in the queue.
 * Don't forget to allocate memory for data. For instance, if data is an int:
 * ____Do____
 * int data;
 * dequeue(my_queue, &data);
 * ____Do not____
 * int *data;
 * dequeue(my_queue, data);
 *
 * @param queue: the queue that will provide its first data
 * @param data: a pointer pointing to the address that will receive the data.
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int dequeue(queue_t *queue, void *data);

/**
 * @brief Check the head of a queue but does not remove it. After calling this
 * function, the queue will remain the same as before.
 *
 * If there is nothing in the queue, -1 will be returned.
 * Don't forget to allocate memory for data. For instance, if data is an int:
 * ____Do____
 * int data;
 * queue_peek(my_queue, &data);
 * ____Do not____
 * int *data;
 * queue_peek(my_queue, data);
 *
 * @param queue: the queue you want to check the head.
 * @param data: a pointer pointing to the address that will receive the data.
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int queue_peek(const queue_t *queue, void *data);

/**
 * @brief Delete a queue and frees all the memory allocated by it.
 *
 * @param queue: the queue will be deleted.
 */
void delete_queue(queue_t *queue);

/**
 * @brief Check if the queue is empty.
 *
 * @param queue: the queue that will be tested.
 * @return an int indicating if the queue is empty:
 * 1 -> the queue is empty.
 * 0 -> the queue is not empty.
 */
int queue_is_empty(const queue_t *queue);

#endif //QUEUE_QUEUE_H
