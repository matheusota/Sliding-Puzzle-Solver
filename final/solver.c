#include "solver_functions.h"

int main(){
	int i, j;
	tree t;
	char buffer[1000];
	int n_buffer = 0;

	printf("Enter the puzzle size(3 or 4):\n");
	scanf("%d", &SIZE);
	
	printf("Select the algorithm:\n");
	printf("0 -> A*\n");
	printf("1 -> IDA*\n");
	scanf("%d", &algorithm_id);
		
	printf("Select the heuristic:\n");
	printf("0 -> Manhattan\n");
	if(SIZE == 4){
		printf("1 -> PDB 5/5/5\n");
		printf("2 -> PDB 3/6/6\n");
	}
	else
		printf("1 -> PDB\n");
	scanf("%d", &heuristic_id);
		
	printf("Please enter the current state of the puzzle:\n");
	
	//scan initial state
	initial = malloc(sizeof(node));
	initial -> parent = NULL;
	initial -> action = 'x';
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			scanf("%d", &initial -> s[i][j]);
	}
	initial -> key = state2number(initial -> s);
	
	//initialize some variables
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			goalState[i][j] = SIZE * i + j;
	}
	directions[0] = 'u';
	directions[1] = 'r';
	directions[2] = 'd';
	directions[3] = 'l';
	expanded = 0;
	generated = 0;
	
	if(heuristic_id > 0){
		//read the pattern database
		printf("reading the pdb...\n");
		readpdb();
	}
	
	//run search
	if(algorithm_id == 0){
		printf("running A*...\n");
		t = astar();
	}
	else{
		printf("running IDA*...\n");
		t = idastar();
	}
	
	freeHash(0);
	freeHash(1);
	freeHash(2);
	freeHash(3);
	
	printf("solution steps:\n");
	
	//traverse to the root
	while(t -> parent != NULL){
		buffer[n_buffer] = t -> action;
		n_buffer++;
		t = t -> parent;
	}
	
	//print in reverse order
	for(i = n_buffer - 1; i >= 0; i--)
		printf("%c ", buffer[i]);
	printf("\n");
	
	printf("%d nodes expanded\n", expanded);
	printf("%d nodes generated\n", generated);
	return 0;
}
