#ifndef _LINKED_LIST_FOR_POLYNOMIAL_ADD_AND_MUL_
#define _LINKED_LIST_FOR_POLYNOMIAL_ADD_AND_MUL_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//	x  > y 이면 1
//	x == y 이면 0
//	x  < y 이면 -1
#define COMPARE(x,y) ((x > y) ? 1 : ((x == y) ? 0 : -1))

typedef struct ListNode
{
	int coef; // coefficient
	int expon; // exponent
	struct ListNode * link;
}ListNode;

typedef struct ListHeader
{
	int length;
	ListNode * head;
	ListNode * tail;
}ListHeader;

void init(ListHeader * plist);

void insert_node_last(ListHeader * plist,int coef,int expon);

void poly_add(ListHeader * plist1,ListHeader * plist2,ListHeader * plist3);

void poly_mul(ListHeader * plist1,ListHeader * plist2,ListHeader * plist3);

void poly_print(ListHeader * plist);

void poly_delete(ListHeader * plist);

#endif
