#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//data structures
typedef int state[3][3];

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

//global variables
tree initial;
state goalState;
char directions[4];

queue_vector queue[10000];
int n_queue;

int expanded;
int generated;

//functions
tree astar();
tree applyAction(state s, char action);
bool checkDuplicate(tree t, tree child);
bool compareStates(state s1, state s2);
int manhattan(state s);
int manhattanTile(state s, int x);
void getTileGoalPosition(int x, int *goal_i, int *goal_j);
void insertQueue(tree t, int g, int h);
void removeQueue(tree *t, int *g, int *h);
void orderQueue();
void printState(state s);
