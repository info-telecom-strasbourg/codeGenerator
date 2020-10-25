#include "its_unit_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

long long __remaining_primsys_its = -1;
long long __remaining_alloc_its = -1;
long long __remaining_threads_fct_its = -1;

struct test_struct{
	int data_int;
	char data_char;
};

void
test_create_queue(void)
{
	queue_s *my_queue = create_queue(sizeof(int));
	assert(my_queue != NULL);
	assert(my_queue->head == NULL);
    assert(my_queue->tail == NULL);
    assert(my_queue->node_memsize == sizeof(int));
	delete_queue(my_queue);
	queue_s *my_queue2 = create_queue(sizeof(long long));
	assert(my_queue2 != NULL);
	assert(my_queue2->head == NULL);
    assert(my_queue2->tail == NULL);
    assert(my_queue2->node_memsize == sizeof(long long));
	delete_queue(my_queue2);
}

void
test_enqueue()
{
	queue_s *my_queue = create_queue(sizeof(int));
	int data = 1;
	assert(enqueue(my_queue, &data)== 0);
	assert(*(int *)my_queue->tail->data == 1);
	int data2 = 2;
	assert(enqueue(my_queue, &data2)== 0);
	assert(*(int *)my_queue->tail->data == 2);
	delete_queue(my_queue);

	queue_s *my_queue2 = create_queue(sizeof(struct test_struct));
	struct test_struct data_struct = { 1, '1'};
	assert(enqueue(my_queue2, &data_struct) == 0);
	assert((*(struct test_struct *)my_queue2->tail->data).data_int == 1);
	assert((*(struct test_struct *)my_queue2->tail->data).data_char == '1');
	delete_queue(my_queue2);
}

void
test_dequeue()
{
	queue_s *my_queue = create_queue(sizeof(int));
	int data = 1;
	enqueue(my_queue, &data);
	int data2 = 2;
	enqueue(my_queue, &data2);

	int dequeued_data1;
	assert(dequeue(my_queue, &dequeued_data1) == 0);
	assert(dequeued_data1 == 1);
	assert(*(int *)my_queue->tail->data == 2);
	int dequeued_data2;
	assert(dequeue(my_queue, &dequeued_data2) == 0);
	assert(dequeued_data2 == 2);
	assert(my_queue->tail == NULL);
	delete_queue(my_queue);

	queue_s *my_queue2 = create_queue(sizeof(struct test_struct));
	struct test_struct data_struct = {1, '1'};
	struct test_struct data_struct2 = {2, '2'};
	enqueue(my_queue2, &data_struct);
	enqueue(my_queue2, &data_struct2);
	struct test_struct data_struct_dequeued;
	struct test_struct data_struct_dequeued2;
	assert(dequeue(my_queue2, &data_struct_dequeued) == 0);
	assert((*(struct test_struct *)my_queue2->tail->data).data_int == 2);
	assert(dequeue(my_queue2, &data_struct_dequeued2) == 0);
	assert(data_struct_dequeued.data_int == 1);
	assert(data_struct_dequeued.data_char == '1');
	assert(data_struct_dequeued2.data_int == 2);
	assert(data_struct_dequeued2.data_char == '2');
	delete_queue(my_queue2);
}

void
test_queue_peek()
{
	queue_s *my_queue = create_queue(sizeof(int));
	int data1 = 1;
	int data2 = 2;
	enqueue(my_queue, &data1);
	enqueue(my_queue, &data2);
	int data_peeked;
	assert(queue_peek(my_queue, &data_peeked) == 0);
	assert(data_peeked == 1);
	int data_dequeued;
	dequeue(my_queue, &data_dequeued);
	assert(data_dequeued == 1);
	delete_queue(my_queue);

	queue_s *my_queue2 = create_queue(sizeof(struct test_struct));
	struct test_struct data_struct = {1, '1'};
	enqueue(my_queue2, &data_struct);
	struct test_struct data_struct_peeked;
	struct test_struct data_struct_dequeued;

	assert(queue_peek(my_queue2, &data_struct_peeked) == 0);
	assert(data_struct_peeked.data_int == 1);
	assert(data_struct_peeked.data_char == '1');

	dequeue(my_queue2, &data_struct_dequeued);
	assert(data_struct_dequeued.data_int == 1);
	delete_queue(my_queue2);
}

void
test_delete_queue()
{
	queue_s *my_queue = create_queue(sizeof(int));
	delete_queue(my_queue);

	queue_s *my_queue2 = create_queue(sizeof(int));
	int data1 = 1;
	enqueue(my_queue2, &data1);
	delete_queue(my_queue2);
}

void
test_is_empty()
{
	queue_s *my_queue = create_queue(sizeof(int));
	assert(queue_is_empty(my_queue));

	int data = 1;
	enqueue(my_queue, &data);
	assert(!queue_is_empty(my_queue));

	int data_dequeued;
	dequeue(my_queue, &data_dequeued);
	assert(queue_is_empty(my_queue));
	delete_queue(my_queue);

	queue_s *my_queue2 = create_queue(sizeof(struct test_struct));
	assert(queue_is_empty(my_queue2));

	struct test_struct data_destruct = { 1, '1' };
	enqueue(my_queue2, &data_destruct);
	assert(!queue_is_empty(my_queue2));

	struct test_struct data_struct_dequeued;
	dequeue(my_queue2, &data_struct_dequeued);
	assert(queue_is_empty(my_queue2));
	delete_queue(my_queue2);
}


int
main(void)
{
    TEST(test_create_queue);
	TEST(test_enqueue);
	TEST(test_dequeue);
	TEST(test_delete_queue);
	TEST(test_is_empty);
}