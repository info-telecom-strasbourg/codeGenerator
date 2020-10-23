#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <stdlib.h>
#include <string.h>

/***************************** How to use a stack ******************************
* A stack is a data structure. It is a FILO (First In Last Out) list. You have *
* to use specific methods to access the data stored in the stack. Each time a  *
* data is added, it is stored at the end of the stack. Each time you access a  *
* data, it is the last one you stored in the stack.                            *
* Example:                                                                     *
*                __                                                            *
* Initial stack:|  |                                                           *
*               |__|                                                           *
* We add a node:                                                               *
* Stack :   node 1                                                             *
*          ________                                                            *
*         | data 1 |                                                           *
*         |________|                                                           *
* We add another node:                                                         *
* Stack :   node 1   <----   node 2                                            *
*          ________         ________                                           *
*         | data 1 | <---- | data 2 |                                          *
*         |________|       |________|                                          *
* We get a value of the stack:                                                 *
* Stack :   node 1                                                             *
*          ________                                                            *
*         | data 1 |                                                           *
*         |________|                                                           *
* We get the data stored in the second node.                                   *
*******************************************************************************/

/**
 * @brief A structure representing a node in the stack.
 *
 * A node store a data (any type) and point to the previous node in the stack.
 */
typedef struct stack_node {
    void              *data; /**< The data stored in the node. It can be
                                  anything. */
    struct stack_node *prev; /**< Pointer to identify the previous node. */
} stack_node_s;

/**
 * @brief A structure representing a stack.
 *
 * A stack is a succession of node. It must be used as FILO (First In Last Out)
 * list. To use a stack correctly, please use the functions of this library to
 * push or pop data.
 * If you choose to use your own functions please create a function that put
 * data at the end of the stack and a function that remove data at the end of
 * the stack.
 */
typedef struct stack {
    size_t       node_memsize; /**< The size of the data will be stored in a
                                    node. */
    stack_node_s *tail; /**< The tail (last node) of the stack. */
} stack_s;

/**
 * @brief Initialize the attributes of a stack.
 *
 * This function check if the node_memsize is not 0 (the size of the data
 * stored can not be  0). Be careful, if you pass a negative parameter,
 * the size_t will convert it to a positive variable and can lead to
 * unexpected behaviour. You may use sizeof(data_type) to have the correct
 * size.
 *
 * @param stack: the stack you want to initialize.
 * @param node_memsize: the size of the data that will be stored in the stack.
 * @return an int that indicate if the creation were done without error:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int create_stack(stack_s *stack, size_t node_memsize);

/**
 * @brief Push a new node at the end of the stack.
 *
 * This function check if memory allocation succeeded. If not, the memory
 * allocated by the function will be freed. The stack will not be freed,
 * just the memory allocated by this call of the function.
 *
 * @param stack: the stack that will receive the new node.
 * @param data: the data that will be stored in the stack;
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int push_stack(stack_s *stack, const void *data);

/**
 * @brief Remove the last node of the stack.
 *
 * The function free the memory allocated when the node was put in the stack.
 * Don't forget to allocate memory for data. For instance, if data is an int:
 * ____Do____
 * int data;
 * pop_stack(my_stack, &data);
 * ____Do not____
 * int *data;
 * pop_stack(my_stack, data);
 *
 * @param stack: the stack that will provide its last node.
 * @param data: a pointer pointing to the address that will receive the data.
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int pop_stack(stack_s *stack, void *data);

/**
 * @brief Check the last node of the stack. After calling this function, the
 * stack will remain the same.
 *
 * Don't forget to allocate memory for data. For instance, if data is an int:
 * ____Do____
 * int data;
 * stack_peek(my_stack, &data);
 * ____Do not____
 * int *data;
 * stack_peek(my_stack, data);
 *
 * @param stack: the stack you want to check the last node.
 * @param data: a pointer pointing to the address that will receive the data.
 * @return an int that indicate if the function succeeded:
 * 0 -> the function succeeded.
 * -1 -> the function failed.
 */
int stack_peek(const stack_s *stack, void *data);

/**
 * @brief Delete a stack and free all the memory allocated by it.
 *
 * @param stack: the stack that will be deleted.
 */
void delete_stack(stack_s *stack);

/**
 * @brief Check if the stack is empty.
 *
 * @param stack: the stack that will be checked.
 * @return an int indicating if the stack is empty:
 * 1 -> the stack is empty.
 * 0 -> the stack is not empty.
 */
int stack_is_empty(const stack_s *stack);

#endif //STACK_STACK_H
