#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define HASH_SIZE 5765731
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
tree initial;
state goalState;
char directions[4];

//queue
queue_vector queue[1000000];
int n_queue;

//hash
hashnode hash[HASH_SIZE];

//counting nodes
int expanded;
int generated;

//functions
tree astar();
bool isGoal(state s);
tree applyAction(state s, char action);
bool checkDuplicate(tree t, tree child);
int getHeuristic(state s);

void insertQueue(tree t, int g, int h);
void removeQueue(tree *t, int *g, int *h);
void orderQueue();

void readpdb();

uint64_t state2number(state s);
void insertHash(uint64_t key, int h);
int searchHash(uint64_t key);

void printState(state s);;
