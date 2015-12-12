#include "list.h"

//create the head of the list
void createList(){
	list new = malloc(sizeof(struct list_aux));
	new -> next = NULL;
	head = new;
	tail = new; 
}

//insert a node on the end of the list
void insertList(void* t, int g, int h){
	list new = malloc(sizeof(struct list_aux));
	new -> g = g;
	new -> h = h;
	new -> t = t;
	new -> next = NULL;
	
	tail -> next = new;
	tail = new;
}

//remove first node of the list
void removeList(void* *t, int *g, int *h){
	list aux = head -> next;
	
	if(aux != NULL){
		*t = aux -> t;
		*g = aux -> g;
		*h = aux -> h;
		
		//this is the last node on the list
		if(aux == tail)
			tail = head;
			
		head -> next = aux -> next;
		free(aux);
	}
}

//check if the list is empty
bool listEmpty(){
	if(head == tail)
		return true;
	else
		return false;
}
