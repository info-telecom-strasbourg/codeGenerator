#ifndef CST_H
#define CST_H

#define NSTUDENTS 5
#include "hashmap.h"

struct student_t
{
	int student_id;
	char name[20];
};

struct prof_t {
	char mat[25];
	int age;
};

struct node_hash_t
{
	unsigned int type;/**< Type associated to a type in union */
	/* Here 1 for prof and 2 for student */
	union /**< Inside, you put all the struct you may want store in a data struct */
	{
		struct prof_t prof;
		struct student_t stud;
	};
};

#endif //CST_H