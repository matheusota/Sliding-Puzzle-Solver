#include "pdb_functions.h"

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

void insertHash(state s, int h){
	uint64_t key = state2number(s);
	int index = (int)(key % HASH_SIZE);
	hashnode new = malloc(sizeof(hashnode));
	
	new -> key = key;
	new -> h = h;
	new -> next = hash[index];
	hash[index] = new;
}

int searchHash(state s){
	uint64_t key = state2number(s);
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

void insertHashBlank(state s, int h){
	uint64_t key = state2number(s);
	int index = (int)(key % HASH_SIZE);
	hashnode new = malloc(sizeof(hashnode));
	
	new -> key = key;
	new -> h = h;
	new -> next = hashBlank[index];
	hashBlank[index] = new;
}

int searchHashBlank(state s){
	uint64_t key = state2number(s);
	int index = (int)(key % HASH_SIZE);
	hashnode aux = hashBlank[index];
	
	while(aux != NULL){
		if(aux -> key == key)
			return aux -> h;
		else
			aux = aux -> next;
	}
	
	return -1;
}

//PDB CREATING FUNCTIONS-----------------------------------------------

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

//get the correct tile position
void getTilePosition(state s, int x, int *goal_i, int *goal_j){
	int i, j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(s[i][j] == x){
				*goal_i = i;
				*goal_j = j;
				return;
			}
		}
	}
}

//create the pdb by doing a bfs on the goal
void createDatabase(){
	tree start, t, child;
	int i, j, g, h, goal_i, goal_j;
    FILE *fp;
   
	inserted = 0;
	
	startTime = clock();
	
	//initialize the pattern
	t = malloc(sizeof(node));
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			t -> s[i][j] = -1;
	}
	t -> s[0][2] = 2;
	t -> s[1][2] = 5;
	t -> s[2][0] = 6;
	t -> s[2][1] = 7;
	t -> s[2][2] = 8;
	
	//initalize file
	fp = fopen("pdb.txt", "w+");
	
	insertHash(t -> s, 0);
	
	//we print into the file in the format "key" "value"
	fprintf(fp, "%llu %d\n", state2number(t -> s), 0);
	
	inserted++;
	
	t -> s[0][0] = 0;
	
	insertHashBlank(t -> s, 0);
	insertQueue(t, 0, 0);
	start = t;
	
	//bfs from the solution to create the pattern database
	while(n_queue > 0){
		//remove the first element of the queue
		removeQueue(&t, &g, &h);
		
		//add each action
		for(i = 0; i < 4; i++){
			child = applyAction(t -> s, directions[i]);
			
			if(child != NULL && searchHashBlank(child -> s) == -1){
				endTime2 = clock();
					
				//insert on the blank hash
				insertHashBlank(child -> s, g + 1);
				
				//insert in the tree
				t -> childs[i] = child;
				child -> parent = t;
				
				//get position of the blank
				getTilePosition(child -> s, 0, &goal_i, &goal_j);
				
				//replace with -1 and insert on the hash
				(child -> s)[goal_i][goal_j] = -1;
				if(searchHash(child -> s) == -1){
					//insert in the hash
					insertHash(child -> s, g + 1);
					
					//we print into the file in the format "key" "value"
					fprintf(fp, "%llu %d\n", state2number(child -> s), g + 1);
					
					printf("%d\n", inserted);
					inserted++;
				}
				
				//return blank to the state
				(child -> s)[goal_i][goal_j] = 0;
				
				//insert in the queue
				insertQueue(child, g + 1, 0);
				
				endTime = clock();
				//printf("time before %f - time after %f\n", ((double) (endTime2 - startTime)) / CLOCKS_PER_SEC, ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
				startTime = endTime;
			}
		}
		
		orderQueue();
	}
	
	//close file
	fclose(fp);
	
	//free the tree
	freeTree(start);
}

//free a tree
void freeTree(tree t){
	int i;
	
	if(t != NULL){
		for(i = 0; i < 4; i++)
			freeTree(t -> childs[i]);
	
		free(t);
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
