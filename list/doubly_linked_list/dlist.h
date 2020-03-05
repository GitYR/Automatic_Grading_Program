#ifndef _DOUBLY_LINKED_LIST_
#define _DOUBLY_LINKED_LIST_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef _Bool bool;

typedef struct _node
{
	int data;
	struct _node * prev; // before
	struct _node * next; // after
}Node;

typedef struct _list
{
	Node * head;
	Node * tail;
}List;

void list_init(List * list); // for initializing

void list_insert(List * list,int data);

bool list_search(List * list,int data); // for searching

int list_delete(List * list,int data);

void list_print(List * list);

bool isEmpty(List * list);

#endif
