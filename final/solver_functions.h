#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "heap.h"

#define HASH_SIZE 5765731

//data structures
typedef int state[4][4];

typedef struct node_aux{
	uint64_t key;
	state s;
	int g;
	char action;
	struct node_aux *childs[4];
	struct node_aux *parent;
} node, *tree;

typedef struct hash_aux{
	uint64_t key;
	int h;
	struct hash_aux *next;
} *hashnode;

//global variables
tree initial;
state goalState;
int SIZE, algorithm_id, heuristic_id;
char directions[4];

//hash
hashnode hash0[HASH_SIZE];
hashnode hash1[HASH_SIZE];
hashnode hash2[HASH_SIZE];
hashnode hashBlank[HASH_SIZE];

//counting nodes
int expanded;
int generated;

//functions
tree astar();
tree idastar();
int dfs(tree t, int bound, int g, tree *result);
bool isGoal(state s);
char oppositeAction(char a);
tree applyAction(state s, char action);
bool checkDuplicate(tree t, tree child);
int getHeuristic(state s);
void getTileGoalPosition(int x, int *goal_i, int *goal_j);
int manhattan(state s);
int manhattanTile(state s, int x);

void readpdb();

uint64_t state2number(state s);
void insertHash(uint64_t key, int h, int id);
int searchHash(uint64_t key, int id);

void freeHash(int id);
void freeListHash(hashnode l);
void freeTree(tree t);

void printState(state s);
