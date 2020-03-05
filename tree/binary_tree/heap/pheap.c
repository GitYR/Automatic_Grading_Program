#include "pheap.h"

void init_heap(Heap * h)
{
	int i;

	h->heap = (int *)malloc(MAX_HEAP*sizeof(int));
	h->heap_size = 0;

	for(i = 1; i <= MAX_HEAP; i++)
		h->heap[i] = INIT_VALUE;
}

void push(Heap * h,int item)
{
	h->heap_size += 1;
	int idx = h->heap_size;
	h->heap[idx] = item;

	while((idx != 1) && (item > h->heap[idx/2]))
	{
		h->heap[idx] = h->heap[idx/2];
		//h->heap[idx/2] = item;
		idx /= 2;
	}
	h->heap[idx] = item; // 개선
}

int pop(Heap * h)
{
	int temp,item = h->heap[1];
	int child = 2,parent = 1;

	//h->heap[1] = h->heap[h->heap_size];
	temp = h->heap[h->heap_size]; // 개선
	h->heap[h->heap_size] = INIT_VALUE;
	h->heap_size -= 1;

	while(child <= h->heap_size)
	{
		if((child < h->heap_size) && (h->heap[child] < h->heap[child+1]))
			child++;
		//if(h->heap[parent] >= h->heap[child])
		if(temp >= h->heap[child]) // 개선
			break;
		//swap(&h->heap[parent],&h->heap[child]); 
		h->heap[parent] = h->heap[child]; // 개선
		parent = child;
		child *= 2;
	}

	h->heap[parent] = temp; // 개선
	return item;
}

void print_heap(Heap * h)
{
	int i = 1;

	while(i <= h->heap_size)
	{
		printf("Heap[%-2d] : %-2d\n",i, h->heap[i]);
		i++;
	}
}

/*
void swap(int * a,int * b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
*/
