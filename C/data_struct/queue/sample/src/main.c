#include <stdio.h>
#include "queue.h"

struct student
{
	char name[25];
	int moy;
};

void fill_queue(queue_ts *stack)
{
	struct student et1 = {"Monsieur", 13};
	enqueue(stack, &et1);
	et1.moy = 15;
	enqueue(stack, &et1);

	struct student etpop;
	queue_peek(stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);

	struct student et2 = {"Madame", 10};
	enqueue(stack, &et2);

	dequeue(stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);
}

int main(void)
{
	queue_ts *my_stack = create_queue(sizeof(struct student));
	fill_queue(my_stack);
	struct student etpop; // sizeof(void *)
	dequeue(my_stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);
	dequeue(my_stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);
	delete_queue(my_stack);
	return 0;
}
