#include <stdio.h>
#include "stack.h"

struct student {
	char name[25];
	int moy;
};

void fill_stack(stack_ts *stack)
{
	struct student et1 = {"Monsieur", 13};
	push_stack(stack, &et1);
	et1.moy = 15;
	push_stack(stack, &et1);

	struct student etpop;
	stack_peek(stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);

	struct student et2 = {"Madame", 10};
	push_stack(stack, &et2);

	pop_stack(stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);
}

int main(void)
{
	stack_ts *my_stack = create_stack(sizeof(struct student));
	fill_stack(my_stack);
	struct student etpop;
	pop_stack(my_stack, &etpop);
	printf("%s\n", etpop.name);
	printf("%d\n", etpop.moy);
	delete_stack(my_stack);
	return 0;
}
