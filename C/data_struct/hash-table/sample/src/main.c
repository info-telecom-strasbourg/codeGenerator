#include <stdio.h>
#include "hashmap.h"
#include "cst.h"

void fill_map(hash_table_ts *table)
{
	int key1 = 1;
	char msg[13] = "Hello World!";
	int key2 = 2;
	char msg2[17] = "Goodnight World!";
	if (insert(table, &key1, msg) == -1)
		exit(EXIT_FAILURE);

	if (insert(table, &key2, msg2) == -1)
		exit(EXIT_FAILURE);
}

void fill_map_2(hash_table_ts *table)
{	
	struct student_t students[NSTUDENTS] = {
		{783682, "John Doe"},
		{783683, "Max Mustermann"},
		{783684, "Cecile Dupont"},
		{783685, "Leopold Patrick"},
		{783686, "Justin Blanc"},
	};

	for (int i = 0; i < NSTUDENTS; i++) {
		if (insert(table, &students[i].name, &students[i]) == -1)
			exit(EXIT_FAILURE);
	}
}

void test1(void)
{
	hash_table_ts *table = create_table(2, sizeof(int), sizeof(char) * 17);
	fill_map(table);
	int key1 = 1;
	char msg[13];
	lookup(table, &key1, msg);
	printf("%s\n", msg);
	int key2 = 2;
	char msg2[17];
	lookup(table, &key2, msg2);
	printf("%s\n", msg2);
	delete_hash_map(table);
}

void test2(void)
{
	hash_table_ts *table = create_table(NSTUDENTS, 20 * sizeof(char), sizeof(struct student_t));
	fill_map_2(table);
	char key1[20] = "Cecile Dupont";
	struct student_t student;
	lookup(table, key1, &student);
	printf("\nStudent\n");
	printf ("%13s %-20d\n", "Student id:", student.student_id);
	printf ("%13s %-20s\n", "Student name:", student.name);
	delete_hash_map(table);
}

int main(void)
{
	test1();
	test2();

	return 0;
}
