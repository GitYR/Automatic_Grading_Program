#ifndef _LINKED_LIST_FOR_STACK_
#define _LINKED_LIST_FOR_STACK_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STACK 10

typedef struct _element
{
	int key;
	/* the other field */
}element;

typedef struct stack * stackPointer;
typedef struct stack
{
	element data;
	stackPointer link;
}stack;

stackPointer top[MAX_STACK];

void push(stackPointer * top,element item);

int pop(stackPointer * top,element * item);

int peek(stackPointer top,element * item);

void printStack(stackPointer top);

#endif
