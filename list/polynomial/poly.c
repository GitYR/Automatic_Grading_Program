#include "poly.h"

void init(ListHeader * plist)
{
	plist->length = 0;
	plist->head = plist->tail = NULL;
}

void insert_node_last(ListHeader * plist,int coef,int expon)
{
	ListNode * temp = (ListNode *)malloc(sizeof(ListNode));
	if(temp == NULL)
	{
		fprintf(stderr,"memory allocating error\n");
		return;
	}

	temp->coef = coef;
	temp->expon = expon;
	temp->link = NULL;

	if(plist->tail == NULL)
		plist->head = plist->tail = temp;

	else // plist->tail != NULL
	{
		plist->tail->link = temp;
		plist->tail = temp;
	}

	plist->length++;
}

void poly_add(ListHeader * plist1,ListHeader * plist2,ListHeader * plist3)
{
	int sum;
	ListNode * a = plist1->head;
	ListNode * b = plist2->head;

	while(a && b)
	{
		switch(COMPARE(a->expon,b->expon))
		{
			case 1 : // when a's exponent is bigger than b's exponent.
				insert_node_last(plist3,a->coef,a->expon);
				a = a->link;
				break;
			case 0 :  // when a's exponent is equal to b's exponent.
				sum = a->coef + b->coef;
				insert_node_last(plist3,sum,a->expon);
				a = a->link; b = b->link;
				break;
			case -1: // the other case.
				insert_node_last(plist3,b->coef,b->expon);
				b = b->link;
				break;
		}
	}

	for(;a;a=a->link) insert_node_last(plist3,a->coef,a->expon);
	for(;b;b=b->link) insert_node_last(plist3,b->coef,b->expon);
}

void poly_mul(ListHeader * plist1,ListHeader * plist2,ListHeader * plist3)
{
	ListNode * a = plist1->head;
	ListNode * b = plist2->head;
	int * mul,highest,i;

	highest = a->expon + b->expon;
	mul = (int *)calloc(highest+1,sizeof(int));

	// a의 한 항에 대해서 b의 모든항을 순회하는 것을 반복
	while(a)
	{
		if(b)
		{
			if(mul[a->expon+b->expon] == 0)
				mul[a->expon+b->expon] = a->coef * b->coef;
			else
				mul[a->expon+b->expon] += a->coef * b->coef;
			b = b->link;
		}
		else // b == NULL
		{
			b = plist2->head;
			a = a->link;
		}
	}

	// 곱셈 연산이 끝나면 mul에 저장되어 있던 계산 결과들을 리스트로 구성
	for(i = highest; i >= 0; i--)
	{
		if(mul[i] != 0)
			insert_node_last(plist3,mul[i],i);
	}
}

void poly_print(ListHeader * plist)
{
	ListNode * p = plist->head;

	for(;p;p = p->link)
	{
		printf("%dX^%d", p->coef,p->expon);
		if(p->link != NULL)
		{
			if(p->link->coef > 0)
				printf(" + ");
			else if(p->link->coef < 0)
				printf(" - ");
		}
	}
	printf("\n");
}

void poly_delete(ListHeader * plist)
{
	ListNode * temp = plist->head;
	ListNode * del;

	while(temp->link)
	{
		del = temp;
		temp = del->link;
		plist->head = temp;
		free(del);
	}
	free(temp);
}
