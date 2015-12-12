#include "solver_functions.h"

//SEARCH FUNCTIONS---------------------------------------------------

//implements A* algorithm
tree astar(){
	tree t, child;
	int i, j, g = 0, h = 0;
	
	n_heap = 0;
	insertHeap(initial, g, h);
	insertHash(initial -> key, 0, 3);
	
	while(n_heap > 0){
		//remove the first element of the queue
		extractHeap((void*)&t, &g, &h);
		
		//increments the number of nodes expanded
		expanded++;
		
		//for each action, add it into the heap
		for(i = 0; i < 4; i++){
			child = applyAction(t -> s, directions[i]);
			
			if(child != NULL && child -> action != oppositeAction(t -> action) && searchHash(child -> key, 3) == -1){
				//insert in the tree
				t -> childs[i] = child;
				child -> parent = t;
				
				//insert in the hash
				insertHash(child -> key, 0, 3);
				
				generated++;
				
				h = getHeuristic(child -> s);	
				
				//goal
				if(isGoal(child -> s))
					return child;
				else
					insertHeap(child, g + 1, h);
			}
		}
	}
	
	return NULL;
}

//implements IDA* algorithm
tree idastar(){
	int new_bound, bound = getHeuristic(initial -> s);
	tree t;
	
	while(true){
		new_bound = dfs(initial, bound, 0, &t);
		
		if(new_bound == -1)
			return t;
		
		else if(new_bound == 99999){
			printf("error: unreachable state\n");
			return NULL;
		}
		
		else
			bound = new_bound;
	}
	
	return NULL;	
}

//depth first search with a depth bound
int dfs(tree t, int bound, int g, tree *result){
	tree child;
	int min, aux, h = getHeuristic(t -> s), i;
	
	expanded++;
	
	//over the bound
	if(g + h > bound)
		return g + h;
	
	//goal
	if(isGoal(t -> s)){
		*result = t;
		return -1;
	}
	
	//branch on each action
	min = 99999;
	for(i = 0; i < 4; i++){
		child = applyAction(t -> s, directions[i]);
		
		if(child != NULL  && child -> action != oppositeAction(t -> action)){
			t -> childs[i] = child;
			child -> parent = t;
			
			aux = dfs(child, bound, g + 1, result);
			generated++;
			
			if(aux == -1)
				return -1;
			
			if(aux < min){
				free(child);
				min = aux;
			}
		}
	}
	
	return min;
}

//check if is the goal state
bool isGoal(state s){
	int i, j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(s[i][j] != goalState[i][j])
				return false;
		}
	}
	
	return true;
}

//find the opposite action
char oppositeAction(char a){
	if(a == 'u')
		return 'd';
	else if(a == 'd')
		return 'u';
	else if(a == 'r')
		return 'l';
	else if(a == 'l')
		return 'r';
	else
		return 'x';
}

//apply an action to a state
tree applyAction(state s, char action){
	int i, j, blank_i, blank_j, aux;
	tree result = malloc(sizeof(node));
	if(result == NULL)
		printf("ended memory!\n");
		
	//copy to the result
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			result -> s[i][j] = s[i][j];
			if(s[i][j] == 0){
				blank_i = i;
				blank_j = j;
			}
		}
	}
	
	//check if its a possible action
	if((action == 'u' && blank_i == 0) || (action == 'r' && blank_j == SIZE - 1) || (action == 'd' && blank_i == SIZE - 1) || (action == 'l' && blank_j == 0))
		return NULL;
	
	//swap tiles
	if(action == 'u'){
		result -> s[blank_i][blank_j] = result -> s[blank_i - 1][blank_j];
		result -> s[blank_i - 1][blank_j] = 0;
	}
	else if(action == 'r'){
		result -> s[blank_i][blank_j] = result -> s[blank_i][blank_j + 1];
		result -> s[blank_i][blank_j + 1] = 0;
	}
	else if(action == 'd'){
		result -> s[blank_i][blank_j] = result -> s[blank_i + 1][blank_j];
		result -> s[blank_i + 1][blank_j] = 0;
	}
	else if(action == 'l'){
		result -> s[blank_i][blank_j] = result -> s[blank_i][blank_j - 1];
		result -> s[blank_i][blank_j - 1] = 0;
	}
	
	result -> action = action;
	result -> key = state2number(result -> s);
	
	for(i = 0; i < 4; i++)
		result -> childs[i] = NULL;
		
	return result;
}

//get the heuristic value
int getHeuristic(state s){
	int i, j;
	state aux0, aux1, aux2;
	
	//manhattan
	if(heuristic_id == 0)
		return manhattan(s);
	
	//pdb
	else{
		//pdb for 8-puzzle
		if(SIZE == 3){
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 2 || s[i][j] == 5 || s[i][j] == 6 || s[i][j] == 7 || s[i][j] == 8)
						aux0[i][j] = s[i][j];
					else
						aux0[i][j] = -1;
				}
			}
			
			return searchHash(state2number(aux0), 0);
		}
		//pdb for 15-puzzle
		else if(heuristic_id == 1){
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 9 || s[i][j] == 12 || s[i][j] == 13 || s[i][j] == 14 || s[i][j] == 15)
						aux0[i][j] = s[i][j];
					else
						aux0[i][j] = -1;
				}
			}
			
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 3 || s[i][j] == 6 || s[i][j] == 7 || s[i][j] == 10 || s[i][j] == 11)
						aux1[i][j] = s[i][j];
					else
						aux1[i][j] = -1;
				}
			}
			
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 1 || s[i][j] == 2 || s[i][j] == 4 || s[i][j] == 5 || s[i][j] == 8)
						aux2[i][j] = s[i][j];
					else
						aux2[i][j] = -1;
				}
			}
			
			return searchHash(state2number(aux0), 0) + searchHash(state2number(aux1), 1) + searchHash(state2number(aux2), 2);
		}
		else if(heuristic_id == 2){
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 12 || s[i][j] == 13 || s[i][j] == 14)
						aux0[i][j] = s[i][j];
					else
						aux0[i][j] = -1;
				}
			}
			
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 3 || s[i][j] == 6 || s[i][j] == 7 || s[i][j] == 10 || s[i][j] == 11 || s[i][j] == 15)
						aux1[i][j] = s[i][j];
					else
						aux1[i][j] = -1;
				}
			}
			
			for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					if(s[i][j] == 1 || s[i][j] == 2 || s[i][j] == 4 || s[i][j] == 5 || s[i][j] == 8 || s[i][j] == 9)
						aux2[i][j] = s[i][j];
					else
						aux2[i][j] = -1;
				}
			}
			
			return searchHash(state2number(aux0), 0) + searchHash(state2number(aux1), 1) + searchHash(state2number(aux2), 2);
		}
	}
}

//get the correct tile position
void getTileGoalPosition(int x, int *goal_i, int *goal_j){
	int i, j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(goalState[i][j] == x){
				*goal_i = i;
				*goal_j = j;
				return;
			}
		}
	}
}

//get the sum of the manhattan distances
int manhattan(state s){
	int i, result = 0;
	
	for(i = 1; i < SIZE * SIZE; i++)
		result = result + manhattanTile(s, i);
		
	return result;
}

//get the manhattan distance of a tile
int manhattanTile(state s, int x){
	int i, j, goal_i, goal_j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(s[i][j] == x){
				getTileGoalPosition(x, &goal_i, &goal_j);
				return abs(i - goal_i) + abs(j - goal_j);
			}
		}
	}
}

//READING PDB FUNCTION----------------------------------------------
void readpdb(){
	FILE *fp0, *fp1, *fp2;
	char line[1000];
	uint64_t key;
	int value;
	
	//read pdb for the 15-puzzle
	if(SIZE == 4 && heuristic_id == 1){
		fp0 = fopen("pdbs/pdb15.5.5.5_0.txt", "r");
		fp1 = fopen("pdbs/pdb15.5.5.5_1.txt", "r");
		fp2 = fopen("pdbs/pdb15.5.5.5_2.txt", "r");
		
		if(fp0 == NULL || fp1 == NULL || fp2 == NULL ){
			printf("error opening the pdb\n");
			return;
		}
		
		while(fgets(line, 1000, (FILE*)fp0) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 0) == -1)
				insertHash(key, value, 0);
		}
		
		while(fgets(line, 1000, (FILE*)fp1) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 1) == -1)
				insertHash(key, value, 1);
		}
		
		while(fgets(line, 1000, (FILE*)fp2) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 2) == -1)
				insertHash(key, value, 2);
		}
	}
	if(SIZE == 4 && heuristic_id == 2){
		fp0 = fopen("pdbs/pdb15.3.6.6_0.txt", "r");
		fp1 = fopen("pdbs/pdb15.3.6.6_1.txt", "r");
		fp2 = fopen("pdbs/pdb15.3.6.6_2.txt", "r");
		
		if(fp0 == NULL || fp1 == NULL || fp2 == NULL ){
			printf("error opening the pdb\n");
			return;
		}
		
		while(fgets(line, 1000, (FILE*)fp0) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 0) == -1)
				insertHash(key, value, 0);
		}
		
		while(fgets(line, 1000, (FILE*)fp1) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 1) == -1)
				insertHash(key, value, 1);
		}
		
		while(fgets(line, 1000, (FILE*)fp2) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 2) == -1)
				insertHash(key, value, 2);
		}
	}
	//read pdb for the 8-puzzle
	else{
		fp0 = fopen("pdbs/pdb8.txt", "r");
		
		if(fp0 == NULL){
			printf("error opening the pdb\n");
			return;
		}
		
		while(fgets(line, 1000, (FILE*)fp0) != NULL){
			sscanf(line, "%llu %d", &key, &value);
			
			if(searchHash(key, 0) == -1)
				insertHash(key, value, 0);
		}
	}
}

//HASHING FUNCTIONS-------------------------------------------------
uint64_t state2number(state s){
	uint64_t idx = 0;
	int i, j, val, array[SIZE * SIZE];
		
	//copy to an array the state
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			array[SIZE * i + j] = s[i][j];
	}

	//computes a value for the state
	for(i = 0; i < SIZE * SIZE; i++){
		val = array[i];
		if(val != -1)
			idx |= ((uint64_t)i) << (val * 4);
	}

  return idx;
}

void insertHash(uint64_t key, int h, int id){
	int index = (int)(key % HASH_SIZE);
	hashnode new = malloc(sizeof(struct hash_aux));
	
	new -> key = key;
	new -> h = h;
	
	if(id == 0){
		new -> next = hash0[index];
		hash0[index] = new;
	}
	else if(id == 1){
		new -> next = hash1[index];
		hash1[index] = new;
	}
	else if(id == 2){
		new -> next = hash2[index];
		hash2[index] = new;
	}
	else{
		new -> next = hashBlank[index];
		hashBlank[index] = new;
	}
}

int searchHash(uint64_t key, int id){
	int index = (int)(key % HASH_SIZE);
	hashnode aux;
	
	if(id == 0)
		aux = hash0[index];
	else if(id == 1)
		aux = hash1[index];
	else if(id == 2)
		aux = hash2[index];
	else
		aux = hashBlank[index];
	
	while(aux != NULL){
		if(aux -> key == key)
			return aux -> h;
		else
			aux = aux -> next;
	}
	
	return -1;
}

//free a hash
void freeHash(int id){
	int i;
	
	if(id == 0){
		for(i = 0; i < HASH_SIZE; i++){
			freeListHash(hash0[i]);
			hash0[i] = NULL;
		}
	}
	else if(id == 1){
		for(i = 0; i < HASH_SIZE; i++){
			freeListHash(hash1[i]);
			hash1[i] = NULL;
		}
	}
	else if(id == 2){
		for(i = 0; i < HASH_SIZE; i++){
			freeListHash(hash2[i]);
			hash2[i] = NULL;
		}
	}
	else{
		for(i = 0; i < HASH_SIZE; i++){
			freeListHash(hashBlank[i]);
			hashBlank[i] = NULL;
		}
	}
}

//free a hashlist
void freeListHash(hashnode l){
	if(l != NULL){
		freeListHash(l -> next);
		free(l);
		l = NULL;
	}
}

//free a tree
void freeTree(tree t){
	int i;
	
	if(t != NULL){
		for(i = 0; i < 4; i++)
			freeTree(t -> childs[i]);
	
		free(t);
		t = NULL;
	}
}

//DEBUG FUNCTIONS---------------------------------------------------
void printState(state s){
	int i, j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			printf("%d ", s[i][j]);
		printf("\n");
	}
}
