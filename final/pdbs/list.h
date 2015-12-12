#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct list_aux{
	int g;
	int h;
	void* t;
	struct list_aux* next;
} *list;

//list
list head, tail;

void createList();
void insertList(void* t, int g, int h);
void removeList(void* *t, int *g, int *h);
bool listEmpty();
