#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CODE 0xFFFFFFFF-1

typedef struct element
{
	int key;
}Element;

typedef _Bool bool;

int rear = -1;
int front = -1;
int MAX_QUEUE = 5; // 배열의 크기

Element * queue;

void copy(Element * start,Element * end,Element * newQueue)
{
	int temp = start - queue;

	for(Element * index = start; index < end; index++)
		newQueue[index-queue-temp] = *index;
}

void printQueue(Element * queue)
{
	int i;

	printf("front:%d, rear:%d\n", front, rear);
	printf("Queue : ");
	if(front == rear)
	{
		printf("\n");
		return;
	}

	i = front;
	while(1)
	{
		i = (i+1) % MAX_QUEUE;
		printf("[%d]%d ", i, queue[i].key);
		if(i == rear)
			break;
	}
	printf("\n");
}

void queueFull(void)
{
	printf("front:%d. rear:%d\n", front, rear);
	fprintf(stderr,"Queue is full, cannot add element\n");
	fprintf(stderr,"resizing the queue\n\n");

	Element * newQueue = (Element *)malloc(sizeof(Element) * MAX_QUEUE * 2);

	/* copy from queue to newQueue */
	int start = (front + 1) % MAX_QUEUE;

	/* 왜 이렇게 나누는지 그림 그려보며 보면 이해가 된다. */
	if(start < 2)
	{
		copy(queue+start,queue+MAX_QUEUE,newQueue);
	}
	else
	{
		copy(queue+start,queue+MAX_QUEUE,newQueue);
		copy(queue,queue+rear,newQueue+MAX_QUEUE-front);
	}

	front = 2 * MAX_QUEUE - 1;
	rear = MAX_QUEUE - 2;
	MAX_QUEUE *= 2;
	free(queue);
	queue = newQueue;
}

Element queueEmpty(void)
{
	Element temp;
	temp.key = ERROR_CODE;
	fprintf(stderr,"Queue is empty, cannot delete element\n");
	return temp;
}

void addq(Element item)
{
	bool check1,check2;
	int prev_rear = rear;

	rear = (rear + 1) % MAX_QUEUE;

	check1 = (front == rear);
	check2 = (front == -1 && rear == 0 && prev_rear == MAX_QUEUE - 1);
	if(check1 || check2)
	{
		queueFull();

		if(check1)
			rear = (rear + 1) % MAX_QUEUE;
		else if(check2)
			rear = (rear + 2) % MAX_QUEUE;
	}
	queue[rear] = item;
}

Element deleteq(void)
{
	if(front == rear)
		return queueEmpty();
	front = (front + 1) % MAX_QUEUE;
	return queue[front];
}


int main(void)
{
	queue = (Element *)malloc(sizeof(Element) * MAX_QUEUE);

	Element item1,item2,item3,item4,item5,item6;

	item1.key = 10;
	item2.key = 20;
	item3.key = 30;
	item4.key = 40;
	item5.key = 50;
	item6.key = 60;

	printf("add 10\n");
	addq(item1);
	printQueue(queue);

	printf("add 20\n");
	addq(item2);
	printQueue(queue);

	printf("add 30\n");
	addq(item3);
	printQueue(queue);

	printf("add 40\n");
	addq(item4);
	printQueue(queue);

	printf("add 50\n");
	addq(item5);
	printQueue(queue);

	printf("delete 10\n");
	deleteq();
	printQueue(queue);

	printf("add 10\n");
	addq(item1);
	printQueue(queue);

	printf("add 60\n");
	addq(item6);
	printQueue(queue);

	/*
	deleteq();
	printQueue();

	deleteq();
	printQueue();
	*/

	free(queue);
	return 0;
}
