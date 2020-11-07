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

#endif //CST_H