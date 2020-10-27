#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<stdio.h>

/************************ How to use a linked list  ****************************
* A linked list is a data structure.                                           *
* You have to use specific methods to access the data stored in the linked     *
* list.                                                                        *
*                                                                              *
* Example:                                                                     *
*                        ________ ________                                     *
* Initial linked list:  |________|________|                                    *
*                                                                              *
* We add a node:              ________ ________                                *
* Linked list :   node 1     |_data_1_|________|                               *
*                                                                              *
*                                                                              *
* We add another node:                                                         *
* Linked list :   node 1   ---->   node 2                                      *
*   ________ ________                 ________ ________                        *
*  |_data_1_|________|     ---->     |_data_2_|________|                       *
*                                                                              *
*******************************************************************************/

//ajouter des codes d'erreur


struct list_node_t;
typedef struct list_node_t list_node_t;


/**
 * @brief A structure representing a node in a linked list.
 *
 * A linked list is a succession of node.
 */

struct list_node_t {
	void        *data; /**< Data (any type) which will be stored in the node*/
	list_node_t *next_node; /**< Pointer to identify the next node. */
};


/**
 * @brief A structure representing a linked list.
 *
 * A linked list is a succession of node.
 * To use a linked list correctly, please use the functions of this library.
 */

typedef struct {
	list_node_t *beginning_list; /**< Pointer to identify the first node of the linked list (point to NULL if it's empty). */
	size_t      size_list; /**< Size of the linked list (0 if it's empty). */
} list_t;


/**
 * @brief Initialize the attributes of a linked list.
 *
 * This function check if the node_memsize is not 0 (the size of the data
 * stored can not be  0) and create an empty list in this case.

 * @return NULL or a pointer to the linked list
 * a pointer to the linked list -> the function succeeded.
 * NULL -> the function failed, the linked list wasn't created.
 */
list_t *list_create(void);


/**
 * @brief Delete a linked list and free all the memory allocated by it.
 *
 * @param list: the linked list that will be deleted.
 */
void list_delete(list_t *list);


/**
 * @brief Check if the linked list is empty.
 *
 * @param list: the linked list that will be tested.
 * @return an int indicating if the linked list is empty:
 * 1 -> the linked list is empty.
 * 0 -> the function failed or the linked list is not empty.
 */
int list_is_empty(list_t *list);


/**
 * @brief Add a data to the end of the linked list.
 * 
 * @param list : linked list where the data will be added.
 * @param ele : data (any type) which will be added.
 * @return an int indicating if the data is added :
 * 1 -> the data is added.
 * 0 -> the function failed.
 */
int list_append(list_t *list, void *ele);


/**
 * @brief Add a data to the head of the linked list.
 * 
 * @param list : linked list where the data will be added.
 * @param ele : data (any type) which will be added.
 * @return an int indicating if the data is added :
 * 1 -> the data is added.
 * 0 -> the function failed.
 */
int list_prepend(list_t *list, void *ele);


/**
 * @brief Remove the last node of the linked list.
 *
 * @param list: the linked list that will provide its last node.
 * @param ele: a pointer pointing to the address that will receive the data.
 * @return an int that indicate if the function succeeded:
 * 1 -> the function succeeded.
 * 0 -> the function failed.
 */
int list_pop(list_t *list, void *ele);


/** 
 * @brief Give the size of the linked list.
 * 
 * @param list: the linked list studied.
 * @return an int, size of the linked list.
 */
int list_size(list_t *list);

#endif