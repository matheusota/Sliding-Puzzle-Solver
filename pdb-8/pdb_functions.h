#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define HASH_SIZE 5765731 //needs to be a prime number
#define SIZE 3

//__uint128_t is the type for integers of 128bits

//data structures
typedef int state[SIZE][SIZE];

typedef struct node_aux{
	state s;
	char action;
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

//queue
queue_vector queue[1000000];
int n_queue;

//hash
hashnode hash[HASH_SIZE];
hashnode hashBlank[HASH_SIZE];

//counting nodes inserted on hash
int inserted;

//functions
void insertQueue(tree t, int g, int h);
void removeQueue(tree *t, int *g, int *h);
void orderQueue();

uint64_t state2number(state s);
void insertHash(state s, int h);
int searchHash(state s);

void insertHashBlank(state s, int h);
int searchHashBlank(state s);
void getTilePosition(state s, int x, int *goal_i, int *goal_j);

void createDatabase();
tree applyAction(state s, char action);
void printState(state s);
void freeTree(tree t);
