#ifndef _MAX_HEAP_
#define _MAX_HEAP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define INIT_VALUE 0xEFFFFFFF

#define MAX_HEAP 100
#define HEAP_FULL(x) (x == MAX_HEAP - 1)

typedef _Bool bool;

typedef struct _heap
{
	int * heap;
	int heap_size;
}Heap;

void init_heap(Heap * h);

void push(Heap * h,int item);

int pop(Heap * h);

void print_heap(Heap * h);

//void swap(int * a,int * b);

#endif
