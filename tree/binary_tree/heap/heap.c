#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 200 

#define HEAP_FULL (n == MAX_ELEMENTS -1) // 0번 인덱스는 비워놓으므로

typedef struct _heapType
{
	int heap[MAX_ELEMENTS];
	int heap_size;
}HeapType;

HeapType * createHeap(void)
{
	HeapType * h = (HeapType *)calloc(MAX_ELEMENTS,sizeof(HeapType));
	h->heap_size = 0;
	return h;
}

void push(HeapType * h,int item)
{
	int i;
	h->heap_size += 1; // heap 사이즈 증가
	i = h->heap_size; // i는 새로 들어올 item의 인덱스 위치

	while((i != 1) && (item > h->heap[i/2])) // 새로 들어오는 item값이 부모노드보다 크고 루트 노드(i == 1)가 아닐 때까지 진행
	{
		h->heap[i] = h->heap[i/2];
		i /= 2;
	}
	h->heap[i] = item;
}

int pop(HeapType * h)
{
	int parent,child;
	int item,temp;

	item = h->heap[1]; // 루트 노드값 저장
	temp = h->heap[h->heap_size]; // heap의 가장 마지막 값
	h->heap_size -= 1; // heap의 크기 1 감소
	parent = 1; 
	child = 2;

	while(child <= h->heap_size) // heap의 마지막 원소까지 진행
	{
		if((child < h->heap_size) && (h->heap[child]) < h->heap[child+1]) // while문 조건에 충족하면서 두 child 중에 더 큰 값과 비교하기 위함
			child++;
		if(temp >= h->heap[child]) // 탈출 조건
			break;
		h->heap[parent] = h->heap[child]; // 교체
		parent = child;
		child *= 2;
	}

	h->heap[parent] = temp; // 정해진 위치에 item 입력하고 끝
	return item;
}

void printHeap(HeapType * h)
{
	int i;
	printf(" Heap : ");
	for(i = 1; i <= h->heap_size; i++)
		printf(" [%d] ", h->heap[i]);
}

int main(void)
{
	int i,n,item;
	HeapType * heap = createHeap();

	push(heap,1);
	push(heap,2);
	push(heap,3);
	push(heap,4);
	push(heap,5);
	push(heap,6);
	push(heap,7);
	push(heap,8);
	push(heap,9);

	printHeap(heap);
	n = heap->heap_size;

	for(i = 1; i <= n; i++)
	{
		item = pop(heap);
		printf(" \n delete : [%d] ", item);
	}
	printf("\n");
	return 0;
}
