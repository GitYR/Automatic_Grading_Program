#include "list_queue.h"

int main(void)
{
	QueueType q;

	init(&q);
	addq(&q,1);
	addq(&q,2);
	addq(&q,3);

	printf("deleteq() = %d\n", deleteq(&q));
	printf("deleteq() = %d\n", deleteq(&q));
	printf("deleteq() = %d\n", deleteq(&q));
	return 0;
}
