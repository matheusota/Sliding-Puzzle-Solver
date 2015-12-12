#include "heap.h"

//swap two places in the heap
void swapHeap(int i, int j){
        heapnode aux = heap[i];
       
        //troca os vertices
        heap[i] = heap[j];
        heap[j] = aux;
}

//fix heap after removing
void heapify(int x){
	int l, r, minor;
	minor = x;
	l = 2 * x + 1;
	r = 2 * x + 2;
   
	if((l < n_heap) && (heap[l].g + heap[l].h < heap[minor].g + heap[minor].h))
			minor = l;
   
	if((r < n_heap) && (heap[r].g + heap[r].h < heap[minor].g + heap[minor].h))
			minor = r;
		   
	if(minor != x){
			swapHeap(minor, x);
			heapify(minor);
	}
}

//extract from heap
void extractHeap(void* *t, int *g, int *h){
	heapnode aux;

	if(n_heap != 0){
		aux = heap[0];
		heap[0] = heap[n_heap - 1];
		n_heap--;
		
		if(n_heap > 0)
			heapify(0);
	}

	*g = aux.g;
	*h = aux.h;
	*t = aux.t;
}

//insert on the heap
void insertHeap(void* t, int g, int h)
{
    int parentnode, location = n_heap;
    heapnode aux;
    aux.g = g;
    aux.h = h;
    aux.t = t;
    
    while (location > 0){
		parentnode = (location - 1)/2;
		
		//check parent
		if (g + h > heap[parentnode].g + heap[parentnode].h){
			heap[location] = aux;
			n_heap++;
			return;
		}

		heap[location] = heap[parentnode];
		location = parentnode;
    }

	//put on the root if no place could be found
    heap[0] = aux;
    n_heap++;
}
