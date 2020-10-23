#include "its_unit_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

long long __remaining_primsys_its = -1;
long long __remaining_alloc_its = -1;
long long __remaining_threads_fct_its = -1;

struct test_struct{
	int data_int;
	char data_char;
};

void
test_create_stack(void)
{
    stack_s my_stack;
    assert(create_stack(&my_stack, sizeof(int)) == 0);
    assert(my_stack.tail == NULL);
    assert(my_stack.node_memsize == sizeof(int));
	delete_stack(&my_stack);
	stack_s my_stack2;
	assert(create_stack(&my_stack2, sizeof(long long)) == 0);
    assert(my_stack2.tail == NULL);
    assert(my_stack2.node_memsize == sizeof(long long));
	delete_stack(&my_stack2);
}

void
test_push_stack(void)
{
	stack_s my_stack;
    create_stack(&my_stack, sizeof(int));
	int data = 1;
	push_stack(&my_stack, &data);
	assert(*(int *)my_stack.tail->data == 1);
	int data2 = 2;
	push_stack(&my_stack, &data2);
	assert(*(int *)my_stack.tail->data == 2);
	delete_stack(&my_stack);

	stack_s my_stack2;
    create_stack(&my_stack2, sizeof(struct test_struct));
	struct test_struct struct_test_data = {1, 'c'};
	push_stack(&my_stack2, &struct_test_data);
	assert((*(struct test_struct *)my_stack2.tail->data).data_int == 1);
	assert((*(struct test_struct *)my_stack2.tail->data).data_char == 'c');
	struct test_struct struct_test_data2 = {2, 'h'};
	push_stack(&my_stack2, &struct_test_data2);
	assert((*(struct test_struct *)my_stack2.tail->data).data_int == 2);
	assert((*(struct test_struct *)my_stack2.tail->data).data_char == 'h');
	delete_stack(&my_stack2);
}

void
test_pop_stack(void)
{
	stack_s my_stack;
    create_stack(&my_stack, sizeof(int));
	int data = 1, data2 = 2;
	push_stack(&my_stack, &data);
	push_stack(&my_stack, &data2);
	int recv_data1, recv_data2;
	pop_stack(&my_stack, &recv_data1);
	assert(recv_data1 == 2);
	pop_stack(&my_stack, &recv_data2);
	assert(recv_data2 == 1);
	delete_stack(&my_stack);

	stack_s my_stack2;
    create_stack(&my_stack2, sizeof(struct test_struct));
	struct test_struct struct_test_data = {1, 'c'};
	struct test_struct struct_test_data2 = {2, 'h'};
	push_stack(&my_stack2, &struct_test_data);
	push_stack(&my_stack2, &struct_test_data2);
	struct test_struct recv_data_struct1, recv_data_struct2;
	pop_stack(&my_stack2, &recv_data_struct1);
	assert(recv_data_struct1.data_int == 2);
	assert(recv_data_struct1.data_char == 'h');

	pop_stack(&my_stack2, &recv_data_struct2);
	assert(recv_data_struct2.data_char == 'c');
	assert(recv_data_struct2.data_int == 1);
	delete_stack(&my_stack2);
}

void
test_stack_peek(void)
{
	stack_s my_stack;
    create_stack(&my_stack, sizeof(int));
	int data = 1, data2 = 2;
	push_stack(&my_stack, &data);
	push_stack(&my_stack, &data2);
	int recv_data1, recv_data2;
	stack_peek(&my_stack, &recv_data1);
	assert(recv_data1 == 2);
	assert(*(int *)my_stack.tail->data == 2);
	pop_stack(&my_stack, &recv_data1);
	stack_peek(&my_stack, &recv_data2);
	assert(recv_data2 == 1);
	assert(*(int *)my_stack.tail->data == 1);
	delete_stack(&my_stack);

	stack_s my_stack2;
    create_stack(&my_stack2, sizeof(struct test_struct));
	struct test_struct struct_test_data = {1, 'c'};
	struct test_struct struct_test_data2 = {2, 'h'};
	push_stack(&my_stack2, &struct_test_data);
	push_stack(&my_stack2, &struct_test_data2);
	struct test_struct recv_data_struct1, recv_data_struct2;
	stack_peek(&my_stack2, &recv_data_struct1);
	assert(recv_data_struct1.data_int == 2);
	assert(recv_data_struct1.data_char == 'h');
	assert((*(struct test_struct *)my_stack2.tail->data).data_int == 2);
	assert((*(struct test_struct *)my_stack2.tail->data).data_char == 'h');
	pop_stack(&my_stack2, &recv_data_struct1);

	stack_peek(&my_stack2, &recv_data_struct2);
	assert(recv_data_struct2.data_int == 1);
	assert(recv_data_struct2.data_char == 'c');
	assert((*(struct test_struct *)my_stack2.tail->data).data_int == 1);
	assert((*(struct test_struct *)my_stack2.tail->data).data_char == 'c');
	delete_stack(&my_stack2);
}

void
test_delete_stack()
{
	stack_s my_stack;
	create_stack(&my_stack, sizeof(int));
	delete_stack(&my_stack);

	stack_s my_stack2;
	int data1 = 1;
	create_stack(&my_stack2, sizeof(int));
	push_stack(&my_stack2, &data1);
	delete_stack(&my_stack2);
	assert(my_stack2.tail == NULL);
}

void
test_is_empty()
{
	stack_s my_stack;
	create_stack(&my_stack, sizeof(int));
	assert(stack_is_empty(&my_stack));

	int data = 1;
	push_stack(&my_stack, &data);
	assert(!stack_is_empty(&my_stack));

	int data_pop_stackd;
	pop_stack(&my_stack, &data_pop_stackd);
	assert(stack_is_empty(&my_stack));
	delete_stack(&my_stack);

	stack_s my_stack2;
	create_stack(&my_stack2, sizeof(struct test_struct));
	assert(stack_is_empty(&my_stack2));

	struct test_struct data_destruct = { 1, '1' };
	push_stack(&my_stack2, &data_destruct);
	assert(!stack_is_empty(&my_stack2));

	struct test_struct data_struct_pop_stackd;
	pop_stack(&my_stack2, &data_struct_pop_stackd);
	assert(stack_is_empty(&my_stack2));
	delete_stack(&my_stack2);
}

int
main(void)
{
    TEST(test_create_stack);
	TEST(test_push_stack);
	TEST(test_pop_stack);
	TEST(test_stack_peek);
	TEST(test_delete_stack);
	TEST(test_is_empty);
}
