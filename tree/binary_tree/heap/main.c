#include "pheap.h"

int main(void)
{
	int i;
	Heap heap;

	srand(time(NULL));

	init_heap(&heap);
	for(i = 10; i > 0; i--)
	{
		int random = rand() % 100;
		push(&heap,random);
	}
	print_heap(&heap);

	for(i = 0; i < 10; i++)
		printf("pop value : %d\n", pop(&heap));

	exit(EXIT_SUCCESS);
}
