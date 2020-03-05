/* 
   다항식 연산(덧셈 및 곱셈) 프로그램
   Visual Studio에서 컴파일 안될 수도 있음
   Terminal환경에서 실행하는 것을 추천
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define COMPARE(x,y) ((x > y) ? 1 : ((x == y) ? 0 : -1))

typedef _Bool bool;

typedef struct _node
{
	int coef;
	int expon;
	struct _node * next;
}Node;

typedef struct _list
{
	Node * head;
}List;

void list_init(List * list)
{
	list->head = NULL;
}

bool isEmpty(List * list)
{
	if(list->head == NULL)
		return TRUE;
	return FALSE;
}

int list_insert(List * list,int coef,int expon)
{
	Node * newNode = (Node *)malloc(sizeof(Node));
	newNode->coef = coef;
	newNode->expon = expon;

	if(isEmpty(list))
	{
		newNode->next = NULL;
		list->head = newNode;
	}
	else
	{
		Node * prev = NULL;
		Node * ptr = list->head;

		while(ptr)
		{
			if(ptr->expon > expon)
			{
				prev = ptr;
				ptr = ptr -> next;
			}
			else if(ptr->expon < expon)
			{
				if(!prev) // prev == NULL
				{
					newNode->next = list->head;
					list->head = newNode;
				}
				else
				{
					prev->next = newNode;
					newNode->next = ptr;
				}
				return 0;
			}
			else
			{
				ptr->coef = coef;
				return 0;
			}
		}
		prev->next = newNode;
		newNode->next = NULL;
		return 0;
	}
	return -1;
}

int list_delete(List * list,int coef,int expon)
{
	Node * prev = NULL;
	Node * ptr = list->head;
	if(isEmpty(list))
		return -1;

	while(ptr)
	{
		if(ptr->expon == expon && ptr->coef == coef)
		{
			if(ptr == list->head)
			{
				list->head = list->head->next;
				free(ptr);
			}
			else // ptr != list->head
			{
				prev->next = ptr->next;
				free(ptr);
			}
			return 0;
		}
		prev = ptr;
		ptr = ptr->next;
	}

	return -1;
}

// 다항식 부호 변경
int poly_minus(List * list)
{
	Node * ptr = list->head;
	if(isEmpty(list))
		return -1;

	while(ptr)
	{
		ptr->coef = -(ptr->coef);
		ptr = ptr->next;
	}
}

// 다항식 덧셈 함수
int poly_add(List * list1,List * list2,List * result)
{
	int coef;
	Node * a = list1->head;
	Node * b = list2->head;

	list_init(result);

	while(a && b)
	{
		switch(COMPARE(a->expon,b->expon))
		{
			case 1: // a > b
				list_insert(result,a->coef,a->expon);
				a = a->next;
				break;
			case 0: // a == b
				coef = a->coef + b->coef;
				if(coef)
					list_insert(result,coef,a->expon);
				a = a->next; b = b->next;
				break;
			case -1: // a < b
				list_insert(result,b->coef,b->expon);
				b = b->next;
				break;
		}
	}
	for(;a;a=a->next)
		list_insert(result,a->coef,a->expon);
	for(;b;b=b->next)
		list_insert(result,b->coef,b->expon);

	return 0;
}

// 다항식 곱셈 함수
int poly_mul(List * list1,List * list2,List * result)
{
	Node * a = list1->head;
	Node * b = list2->head;

	list_init(result);

	int highest = a->expon + b->expon;
	int * mul = (int *)calloc(highest+1,sizeof(int));

	// a의 한 항에 대해서 b의 모든 항들을 순회
	while(a)
	{
		if(b)
		{
			if(!mul[a->expon+b->expon])
				mul[a->expon+b->expon] = a->coef * b->coef;
			else
				mul[a->expon+b->expon] += a->coef * b->coef;
			b = b->next;
			continue;
		}
		b = list2->head;
		a = a->next;
	}

	for(int i = 0; i <= highest; i++)
		list_insert(result,mul[i],i);
}

int list_print(List * list)
{
	Node * ptr = list->head;
	if(isEmpty(list))
	{
		fprintf(stderr,"List is empty!\n");
		return -1;
	}

	while(ptr)
	{
		printf("%+dX^%d ", ptr->coef, ptr->expon);
		ptr = ptr->next;
	}
	printf("\n");
	return 0;
}

int main(void)
{
	List list1,list2,add_result,mul_result;

	list_init(&list1);
	list_init(&list2);

	list_insert(&list1,2,2);
	list_insert(&list1,1,1);
	list_insert(&list1,3,0);

	list_insert(&list2,4,2);
	list_insert(&list2,2,1);
	list_insert(&list2,6,0);

	printf("LIST ADD\n");
	poly_add(&list1,&list2,&add_result);
	list_print(&add_result);

	printf("LIST MUL\n");
	poly_mul(&list1,&list2,&mul_result);
	list_print(&mul_result);

	exit(EXIT_SUCCESS);
}
