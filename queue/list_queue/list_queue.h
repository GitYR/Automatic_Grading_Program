#ifndef _LINKED_LIST_FOR_QUEUE_
#define _LINKED_LIST_FOR_QUEUE_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int element;

typedef struct QueueNode
{
	element item;
	struct QueueNode *link;
}QueueNode;

typedef struct QueueType
{
	QueueNode * front;
	QueueNode * rear;
}QueueType;

void error(char * message);

void init(QueueType * q);

int is_empty(QueueType * q);

void addq(QueueType * q,element item);

element deleteq(QueueType * q);

element peek(QueueType * q);

#endif
