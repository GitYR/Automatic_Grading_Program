#include "list_queue.h"

void error(char * message)
{
	fprintf(stderr,"%s\n", message);
	exit(1);
}

void init(QueueType * q)
{
	q->front = q->rear = NULL; // initializing
}

int is_empty(QueueType * q)
{
	return (q -> front == NULL); // if q's front is NULL, it means the queue is empty.
}

void addq(QueueType * q,element item)
{
	QueueNode * temp = (QueueNode *)malloc(sizeof(QueueNode));

	if(temp == NULL)
		error("can not allocate the memory");
	else
	{
		temp->item = item;
		temp->link = NULL;

		if(is_empty(q))
		{
			q->front = temp;
			q->rear = temp;
		}
		else
		{
			q->rear->link = temp;
			q->rear = temp;
		}
	}
}

element deleteq(QueueType * q)
{
	element item;
	QueueNode * temp = q -> front;

	if(is_empty(q))
		error("queue is empty now");

	item = temp -> item;
	q -> front = q -> front -> link;
	if(q->front == NULL)
		q->rear = NULL;
	free(temp);
	return item;
}

element peek(QueueType * q)
{
	if(is_empty(q))
		error("the queue is empty");
	return q->front->item;
}
