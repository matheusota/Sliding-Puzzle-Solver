#include <stdio.h>
#include <stdlib.h>

typedef struct heap_aux{
	int g;
	int h;
	void* t;
} heapnode;

//heap
heapnode heap[10000000];
int n_heap;

void swapHeap(int i, int j);
void heapify(int x);
void extractHeap(void* *t, int *g, int *h);
void insertHeap(void* t, int g, int h);
