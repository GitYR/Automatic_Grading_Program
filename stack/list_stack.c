#include "list_stack.h"

void push(stackPointer * top,element item)
{
	stackPointer temp;
	temp = (stackPointer)malloc(sizeof(*temp));
	if(temp == NULL)
	{
		fprintf(stderr,"error on memory allocating\n");
		exit(1);
	}

	temp->data = item;
	temp->link = *top;
	*top = temp;
}

int pop(stackPointer * top,element * item)
{
	stackPointer temp = *top;

	//item->key = -1;
	if(!temp)
		return 0;

	*item = temp->data;
	*top = temp->link;
	free(temp);

	return 1;
}

int peek(stackPointer top,element * item)
{
	if(top == NULL)
		return 0;
	else
	{
		*item = top->data;
		return 1;
	}
}

void printStack(stackPointer top)
{
	printf("The stack contains: ");
	for(;top ; top = top->link)
		printf("%4d ",top->data.key);
	printf("\n");
}
