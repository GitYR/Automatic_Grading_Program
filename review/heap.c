#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* 최대 힙 */

#define MAX_ELEMENTS 128
#define TRUE 1
#define FALSE 0
#define IS_HEAP_FULL(n) (n >= MAX_ELEMENTS-1 ? TRUE : FALSE)
#define IS_HEAP_EMPTY(n) (n == 0 ? TRUE : FALSE)
#define ERROR_CODE 0x7FFFFFFF

typedef struct heapType
{
	int heap[MAX_ELEMENTS];
	int heap_size;
}heapType;

heapType * create_heap(void)
{
	heapType * h = (heapType *)malloc(sizeof(heapType));
	h->heap_size = 0;
	return h;
}

heapType * resize_heap(heapType * h)
{
}

/* using a priority queue */
void push(heapType * h,int item)
{
	int i;

	if(IS_HEAP_FULL(h->heap_size))
	{
		fprintf(stderr,"heap is full now\n");
		h = resize_heap(h);
		return;
	}

	h->heap_size++; // 힙의 개수 증가
	i = h->heap_size;
	h->heap[i] = item;

	while(i > 1)
	{
		if(item > h->heap[i/2])
		{
			h->heap[i] = h->heap[i/2];
			h->heap[i/2] = item;
		}
		i /= 2;
	}
}

int pop(heapType * h)
{
	int temp,item;
	int parent = 1,child = 2;

	if(IS_HEAP_EMPTY(h->heap_size))
	{
		fprintf(stderr,"heap is empty now\n");
		return ERROR_CODE;
	}
	item = h->heap[1];
	temp = h->heap[h->heap_size];
	h->heap_size--;

	while(child <= h->heap_size)
	{
		if((child < h->heap_size) && h->heap[child] < h->heap[child+1])
			child++;
		if(temp > h->heap[child])
			break;
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}

	h->heap[parent] = temp;
	return item;
}

void print_heap(heapType * h)
{
	int i = 1;
	printf(">>HEAP\n");
	while(i <= h->heap_size)
	{
		printf("[%d] : %d ",i, h->heap[i]);
		i++;
	}
	printf("\n");
}

int main(void)
{
	int i;
	heapType * h = create_heap();

	for(i = 0; i < 10; i++)
		push(h,i);
	print_heap(h);
	printf("\n\n");

	for(i = 0; i < 10; i++)
	{
		printf("pop value : %d\n", pop(h));
		print_heap(h);
	}

	exit(EXIT_SUCCESS);
}
