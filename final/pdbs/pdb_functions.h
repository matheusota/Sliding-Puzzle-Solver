#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "list.h"

#define HASH_SIZE 5765731 //needs to be a prime number

//data structures
typedef int state[4][4];

typedef struct node_aux{
	state s;
	uint64_t key;
	char action;
	int x; //x holds the value of the pattern on the pdb
	struct node_aux *childs[4];
	struct node_aux *parent;
} node, *tree;

typedef struct queue_aux{
	int g;
	int h;
	tree t;
} queue_vector;

typedef struct hash_aux{
	uint64_t key;
	int h;
	struct hash_aux *next;
} *hashnode;

//global variables
char directions[4];
clock_t startTime, endTime, endTime2;
int SIZE;

//hash
hashnode hash[HASH_SIZE];
hashnode hashBlank[HASH_SIZE];

//counting nodes inserted on hash
int inserted;

uint64_t state2number(state s);
void insertHash(state s, int h);
int searchHash(state s);

void insertHashBlank(uint64_t key);
int searchHashBlank(uint64_t key);
void getTilePosition(state s, int x, int *goal_i, int *goal_j);

char oppositeAction(char a);
void createDatabase(int id);
tree applyAction(state s, char action, int x);
void printState(state s);
void freeHash(int id);
void freeListHash(hashnode l);
