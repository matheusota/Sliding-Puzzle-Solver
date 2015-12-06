#include "solver_functions.h"

//SEARCH FUNCTIONS---------------------------------------------------

//implements A* algorithm
tree astar(){
	tree t, child;
	int i, j, g = 0, h = 0;
	
	insertQueue(initial, g, h);
	
	while(n_queue > 0){
		//remove the first element of the queue
		removeQueue(&t, &g, &h);
		
		//increments the number of nodes expanded
		expanded++;
		
		//for each action, add it into the queue
		for(i = 0; i < 4; i++){
			child = applyAction(t -> s, directions[i]);
			
			if(child != NULL && !checkDuplicate(initial, child)){
			
				//insert in the tree
				t -> childs[i] = child;
				child -> parent = t;
				generated++;
				
				h = getHeuristic(child -> s);	
				//goal
				if(isGoal(child -> s))
					return child;
					
				else
					insertQueue(child, g + 1, h);
			}
		}
		
		orderQueue();
	}
	
	return NULL;
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
	
	for(i = 0; i < 4; i++)
		result -> childs[i] = NULL;
		
	return result;
}

//checks if it finds a duplicate state
bool checkDuplicate(tree t, tree child){
	int i;

	if(t != NULL){
		if(state2number(t -> s) == state2number(child -> s))
			return true;
		
		for(i = 0; i < 4; i++){
			if(checkDuplicate(t -> childs[i], child))
				return true;
		}
	}
	
	return false;
}

//get the heuristic value
int getHeuristic(state s){
	int i, j, h;
	state aux;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(s[i][j] == 2 || s[i][j] == 5 || s[i][j] == 6 || s[i][j] == 7 || s[i][j] == 8)
				aux[i][j] = s[i][j];
			else
				aux[i][j] = -1;
		}
	}
	
	h = searchHash(state2number(aux));
	
	if(h == -1){
		printf("error %llu not found on pdb\n", state2number(aux));
		printState(aux);
		return 9999;
	}
	else
		return h;
}

//QUEUE FUNCTIONS---------------------------------------------------

//insert in the end of the queue
void insertQueue(tree t, int g, int h){
	queue[n_queue].t = t;
	queue[n_queue].g = g;
	queue[n_queue].h = h;
	
	n_queue++;
}

//remove the first element from the queue
void removeQueue(tree *t, int *g, int *h){
	int i;
	
	*t = queue[0].t;
	*g = queue[0].g;
	*h = queue[0].h;
	//shift the elements
	for(i = 0; i < n_queue; i++){
		queue[i].t = queue[i+1].t;
		queue[i].g = queue[i+1].g;
		queue[i].h = queue[i+1].h;
	
	}
	
	n_queue--;
}

//order queue  according to f value
void orderQueue(){
	int i, j;
	queue_vector a;
	
	for(i = 0; i < n_queue; i++){
        for(j = i + 1; j < n_queue; j++){
            if(queue[i].g + queue[i].h > queue[j].g + queue[j].h){
                a.g = queue[i].g;
                a.h = queue[i].h;
                a.t = queue[i].t;
                queue[i].g = queue[j].g;
                queue[i].h = queue[j].h;
                queue[i].t = queue[j].t;
                queue[j].g = a.g;
                queue[j].h = a.h;
                queue[j].t = a.t;
            }
        }
    }
}
//READING PDB FUNCTION----------------------------------------------
void readpdb(){
	FILE *fp;
	char line[1000];
	uint64_t key;
	int value;
	
	fp = fopen("pdb.txt", "r");
	
	if(fp == NULL){
		printf("error opening the pdb\n");
		return;
	}
	
	while(fgets(line, 1000, (FILE*)fp) != NULL){
		sscanf(line, "%llu %d", &key, &value);
		
		if(searchHash(key) == -1)
			insertHash(key, value);
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

void insertHash(uint64_t key, int h){
	int index = (int)(key % HASH_SIZE);
	hashnode new = malloc(sizeof(hashnode));
	
	new -> key = key;
	new -> h = h;
	new -> next = hash[index];
	hash[index] = new;
}

int searchHash(uint64_t key){
	int index = (int)(key % HASH_SIZE);
	hashnode aux = hash[index];
	
	while(aux != NULL){
		if(aux -> key == key)
			return aux -> h;
		else
			aux = aux -> next;
	}
	
	return -1;
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
