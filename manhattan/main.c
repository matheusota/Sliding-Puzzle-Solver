#include "functions.h"

int main(){
	int i, j;
	tree t;
	char buffer[1000];
	int n_buffer = 0;
	
	printf("Please enter the current state of the puzzle:\n");
	
	initial = malloc(sizeof(node));
	initial -> parent = NULL;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			scanf("%d", &initial -> s[i][j]);
	}
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			goalState[i][j] = 3 * i + j;
	}
	
	strcpy(directions, "urdl");
	
	expanded = 0;
	generated = 0;
	t = astar();
	printf("Solution steps:\n");
	
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
