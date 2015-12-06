#include "solver_functions.h"

int main(){
	int i, j;
	tree t;
	char buffer[1000];
	int n_buffer = 0;
	
	printf("Please enter the current state of the puzzle:\n");
	
	//scan initial state
	initial = malloc(sizeof(node));
	initial -> parent = NULL;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			scanf("%d", &initial -> s[i][j]);
	}
	
	//initialize some variables
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			goalState[i][j] = SIZE * i + j;
	}
	
	goalState[0][0] = 0;
	strcpy(directions, "urdl");
	expanded = 0;
	generated = 0;
	
	//read the pattern database
	printf("reading the pdb...\n");
	readpdb();
	
	//run astar
	printf("running A*...\n");
	t = astar();
	printf("solution steps:\n");
	while(t -> parent != NULL){
		buffer[n_buffer] = t -> action;
		n_buffer++;
		t = t -> parent;
	}
	
	for(i = n_buffer - 1; i >= 0; i--)
		printf("%c ", buffer[i]);
	printf("\n");
	
	printf("%d nodes expanded\n", expanded);
	printf("%d nodes generated\n", generated);
	return 0;
}
