#include "list_stack.h"

int main(void)
{
	stackPointer top = NULL;

	element data1,data2,data3,data4;
	element temp;

	data1.key = 10;
	data2.key = 20;
	data3.key = 30;
	data4.key = 40;

	push(&top,data1);
	printf("push : %4d\n", data1.key);
	push(&top,data2);
	printf("push : %4d\n", data2.key);
	peek(top,&temp);
	printf("peek : %4d\n", temp.key);
	push(&top,data3);
	printf("push : %4d\n", data3.key);
	push(&top,data4);
	printf("push : %4d\n", data4.key);

	printStack(top);
	peek(top,&temp);
	printf("push : %4d\n", temp.key);
}
