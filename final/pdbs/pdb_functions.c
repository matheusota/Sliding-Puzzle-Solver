#include "pdb_functions.h"

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
	hashnode new = malloc(sizeof(struct hash_aux));
	if(new == NULL)
		printf("ended memory!\n");
		
	new -> key = key;
	new -> h = h;
	new -> next = hash[index];
	hash[index] = new;
}

int searchHash(state s){
	uint64_t key = state2number(s);
	int index = (int)(key % HASH_SIZE);
	hashnode aux = hash[index];
	
	startTime = clock();
	
	while(aux != NULL){
		if(aux -> key == key)
			return aux -> h;
			
		else
			aux = aux -> next;
	}
	
	return -1;
}

void insertHashBlank(uint64_t key){
	int index = (int)(key % HASH_SIZE);
	hashnode new = malloc(sizeof(struct hash_aux));
	if(new == NULL)
		printf("ended memory!\n");
		
	new -> key = key;
	new -> next = hashBlank[index];
	hashBlank[index] = new;
}

int searchHashBlank(uint64_t key){
	int index = (int)(key % HASH_SIZE);
	hashnode aux = hashBlank[index];
	
	startTime = clock();
	
	while(aux != NULL){
		if(aux -> key == key)
			return 1;
			
		else
			aux = aux -> next;
	}
			
	return -1;
}

//PDB CREATING FUNCTIONS-----------------------------------------------

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
tree applyAction(state s, char action, int x){
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
		//moved a number tile
		if(result -> s[blank_i - 1][blank_j] != -1)
			x++;
			
		result -> s[blank_i][blank_j] = result -> s[blank_i - 1][blank_j];
		result -> s[blank_i - 1][blank_j] = 0;
	}
	else if(action == 'r'){
		//moved a number tile
		if(result -> s[blank_i][blank_j + 1] != -1)
			x++;
			
		result -> s[blank_i][blank_j] = result -> s[blank_i][blank_j + 1];
		result -> s[blank_i][blank_j + 1] = 0;
	}
	else if(action == 'd'){
		//moved a number tile
		if(result -> s[blank_i + 1][blank_j] != -1)
			x++;
			
		result -> s[blank_i][blank_j] = result -> s[blank_i + 1][blank_j];
		result -> s[blank_i + 1][blank_j] = 0;
	}
	else if(action == 'l'){
		//moved a number tile
		if(result -> s[blank_i][blank_j - 1] != -1)
			x++;
			
		result -> s[blank_i][blank_j] = result -> s[blank_i][blank_j - 1];
		result -> s[blank_i][blank_j - 1] = 0;
	}
	
	result -> action = action;
	result -> x = x;
	result -> key = state2number(result -> s);
	
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
void createDatabase(int id){
	tree start, t, child;
	int i, j, g, h, blank_i, blank_j;
    FILE *fp;

	createList();
	
	//initialize the pattern
	t = malloc(sizeof(node));
	for(i = 0; i < 4; i++)
		t -> childs[i] = NULL;
		
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			t -> s[i][j] = -1;
	}
	
	if(id == 0){
		t -> s[0][2] = 2;
		t -> s[1][2] = 5;
		t -> s[2][0] = 6;
		t -> s[2][1] = 7;
		t -> s[2][2] = 8;
		
		//initalize file
		fp = fopen("pdb8.txt", "w+");
	}
	else if(id == 1){
		t -> s[3][0] = 12;
		t -> s[3][1] = 13;
		t -> s[3][2] = 14;
		t -> s[3][3] = 15;
		t -> s[2][1] = 9;
		
		//initalize file
		fp = fopen("pdb15_0.txt", "w+");
	}
	
	else if(id == 2){
		t -> s[0][3] = 3;
		t -> s[1][2] = 6;
		t -> s[1][3] = 7;
		t -> s[2][2] = 10;
		t -> s[2][3] = 11;
		
		//initalize file
		fp = fopen("pdb15_1.txt", "w+");
	}
	
	else{
		t -> s[0][1] = 1;
		t -> s[0][2] = 2;
		t -> s[1][0] = 4;
		t -> s[1][1] = 5;
		t -> s[2][0] = 8;
		
		//initalize file
		fp = fopen("pdb15_2.txt", "w+");
	}
	
	insertHash(t -> s, 0);
	
	//we print into the file in the format "key" "value"
	fprintf(fp, "%llu %d\n", state2number(t -> s), 0);
	
	inserted++;
	
	t -> s[0][0] = 0;
	t -> x = 0;
	
	t -> key = state2number(t -> s);
	insertHashBlank(t -> key);
	insertList(t, 0, 0);
	start = t;
	
	//bfs from the solution to create the pattern database
	while(!listEmpty()){
		//remove the first element of the queue
		removeList((void**)&t, &g, &h);
		
		//add each action
		for(i = 0; i < 4; i++){
			child = applyAction(t -> s, directions[i], t -> x);
				
			if(child != NULL && child -> action != oppositeAction(t -> action) && searchHashBlank(child -> key) == -1){
				endTime2 = clock();
				//insert on the blank hash
				insertHashBlank(child -> key);
				
				//insert in the tree
				t -> childs[i] = child;
				child -> parent = t;
				
				//get position of the blank
				getTilePosition(child -> s, 0, &blank_i, &blank_j);
				
				//replace with -1 and insert on the hash
				(child -> s)[blank_i][blank_j] = -1;
				if(searchHash(child -> s) == -1){
					//insert in the hash
					insertHash(child -> s, g + 1);

					//we print into the file in the format "key" "value"
					if(SIZE == 3)
						fprintf(fp, "%llu %d\n", state2number(child -> s), g + 1);
					else
						fprintf(fp, "%llu %d\n", state2number(child -> s), child -> x);
					
					inserted++;
				}
				
				//return blank to the state
				(child -> s)[blank_i][blank_j] = 0;
				
				//insert in the queue
				insertList(child, g + 1, 0);
			}
		}
	}
	
	//close file
	fclose(fp);
	
	//free the tree
	free(start);
}

//free a hash
void freeHash(int id){
	int i;
	
	if(id == 0){
		for(i = 0; i < HASH_SIZE; i++){
			freeListHash(hash[i]);
			hash[i] = NULL;
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

//DEBUG FUNCTIONS---------------------------------------------------
void printState(state s){
	int i, j;
	
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++)
			printf("%d ", s[i][j]);
		printf("\n");
	}
}
