#include <stdio.h>
#include "cst.h"
#include "hashmap.h"

int comp_name(void *a, void *b)
{
	return strcmp((char *)a, (char *)b) == 0;
}

unsigned long long hash_func_name(hash_table_ts *table, void *key)
{
	return strlen((char *)key) % table->size;
}

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
	hash_table_ts *table = create_table(2, sizeof(int), sizeof(char[20]));
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
	printf ("%s %d\n", "Student id:", student.student_id);
	printf ("%s %s\n", "Student name:", student.name);
	delete_hash_map(table);
}

void test3(void)
{
	hash_table_ts *table = create_table(1, sizeof(char[20]), sizeof(struct prof_t));
	struct prof_t profs[3] = {
		{"Math", 26},
		{"Mrançais", 28},
		{"Histoire Géo", 30}
	};

	char name_math[20] = "Théo";
	char name_fr[20] = "TLucas";
	char name_matlab[20] = "Hugo";
	insert(table, name_math, &profs[0]);
	insert(table, name_fr, &profs[1]);
	insert(table, name_matlab, &profs[2]);
	

	struct prof_t prof;
	lookup(table, name_math, &prof);
	printf("%s\n", prof.mat);

	lookup(table, name_fr, &prof);
	printf("%s\n", prof.mat);
	
	lookup(table, name_matlab, &prof);
	printf("%s\n", prof.mat);

	delete_hash_map(table);
}

void test4(void)
{
	hash_table_ts *table = create_table(100, sizeof(char[20]), sizeof(struct node_hash_t));

	struct node_hash_t nodes = { 0 };
	memcpy(nodes.prof.mat, "Math", sizeof(nodes.prof.mat));
	nodes.prof.age = 26;
	nodes.type = 1;
	insert(table, "Théo", &nodes);

	struct node_hash_t nodes2 = { 0 };
	memcpy(nodes2.stud.name, "Mathieu", sizeof(nodes2.stud.name));
	nodes2.stud.student_id = 26123;
	nodes2.type = 2;
	insert(table, "Mathieu", &nodes2);

	struct node_hash_t recup;
	lookup(table, "Mathieu", &recup);
	if(recup.type == 1)
	{
		printf("Prof matière: %s\n", recup.prof.mat);
		printf("Prof age: %d\n", recup.prof.age);
	}
	if(recup.type == 2)
	{
		printf("Student name: %s\n", recup.stud.name);
		printf("Student id: %d\n", recup.stud.student_id);
	}
	

	delete_hash_map(table);
}

int main(void)
{
	test1();
	test2();
	test3();
	test4();

	return 0;
}
