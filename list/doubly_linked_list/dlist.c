#include "dlist.h"

void list_init(List * list)
{
	list->head = list->tail = NULL;
}

void list_insert(List * list,int data)
{
	Node * newNode = (Node *)malloc(sizeof(Node));
	newNode->data = data;

	if(isEmpty(list)) // empty
	{
		list->head = list->tail = newNode;
		list->head->prev = list->head->next = list->tail;
		list->tail->prev = list->tail->next = list->head;
	}
	else // not empty
	{
		list->head->prev = newNode;
		list->tail->next = newNode;
		newNode->prev = list->tail;
		newNode->next = list->head;
		list->tail = newNode;
	}
}

bool list_search(List * list,int data)
{
	Node * temp = list->head;

	if(isEmpty(list))
		return FALSE;

	while(temp)
	{
		if(temp->data == data)
			return TRUE;
		if(temp == list->tail)
			return FALSE;
		temp = temp -> next;
	}
}

int list_delete(List * list,int data)
{
	Node * temp = list->head;
	if(isEmpty(list))
	{
		fprintf(stderr,"The list is empty\n");
		return -1;
	}

	while(temp)
	{
		if(temp -> data == data)
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;

			if(temp == list->head && temp == list->tail)
				list->head = list->tail = NULL;
			else if(temp == list->head)
				list->head = list->head->next;
			else if(temp == list->tail)
				list->tail = temp->prev;

			free(temp);
			return data;
		}
		if(temp == list->tail)
		{
			fprintf(stderr,"There isn't %d in list\n", data);
			return -1;
		}
		temp = temp -> next;
	}
}

void list_print(List * list)
{
	int i = 0;
	Node * temp = list->head;
	if(isEmpty(list))
		return (void)0;

	while(temp)
	{
		printf("[%2d]:%3d  ", ++i, temp->data);
		if(temp == list->tail)
		{
			printf("\n");
			return (void)0;
		}
		temp = temp -> next;
	}
}

bool isEmpty(List * list)
{
	if(list->head == NULL)
		return TRUE;
	return FALSE;
}
