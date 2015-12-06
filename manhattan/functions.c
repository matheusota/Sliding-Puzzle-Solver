#include "functions.h"

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
				
				h = manhattan(child -> s);	
				//goal
				if(h == 0)
					return child;
				else
					insertQueue(child, g + 1, h);
			}
		}
		
		orderQueue();
	}
	
	return NULL;
}

//apply an action to a state
tree applyAction(state s, char action){
	int i, j, blank_i, blank_j, aux;
	tree result = malloc(sizeof(node));
	if(result == NULL)
		printf("ended memory!\n");
	//copy to the result
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			result -> s[i][j] = s[i][j];
			if(s[i][j] == 0){
				blank_i = i;
				blank_j = j;
			}
		}
	}
	
	//check if its a possible action
	if((action == 'u' && blank_i == 0) || (action == 'r' && blank_j == 2) || (action == 'd' && blank_i == 2) || (action == 'l' && blank_j == 0))
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
	return result;
}

//checks if it finds a duplicate state
bool checkDuplicate(tree t, tree child){
	int i;

	if(t != NULL){
		if(compareStates(t -> s, child -> s))
			return true;
		
		for(i = 0; i < 4; i++){
			if(checkDuplicate(t -> childs[i], child))
				return true;
		}
	}
	
	return false;
}

//compare two states
bool compareStates(state s1, state s2){
	int i, j;
	
	for(i = 0; i < 3; i ++){
		for(j = 0; j < 3; j++){
			if(s1[i][j] != s2[i][j])
				return false;
		}
	}
	
	return true;
}

//get the sum of the manhattan distances
int manhattan(state s){
	int i, result = 0;
	
	for(i = 1; i <= 8; i++)
		result = result + manhattanTile(s, i);
		
	return result;
}
//get the manhattan distance of a tile
int manhattanTile(state s, int x){
	int i, j, goal_i, goal_j;
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			if(s[i][j] == x){
				getTileGoalPosition(x, &goal_i, &goal_j);
				return abs(i - goal_i) + abs(j - goal_j);
			}
		}
	}
}

//get the correct tile position
void getTileGoalPosition(int x, int *goal_i, int *goal_j){
	int i, j;
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			if(goalState[i][j] == x){
				*goal_i = i;
				*goal_j = j;
				return;
			}
		}
	}
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

//DEBUG FUNCTIONS---------------------------------------------------
void printState(state s){
	int i, j;
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			printf("%d ", s[i][j]);
		printf("\n");
	}
}
