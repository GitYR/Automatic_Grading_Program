/*************** COPYRIGHT *********************

CODE WRITER : Jiman Hong, SoongSil University

WRITTEN TIME : 2019.9.25

REFERENCE : DATA STRUCTURE LECTURE NOTE

***********************************************/

#ifndef _LINKED_LIST_FOR_POLYNOMIAL_
#define _LINKED_LIST_FOR_POLYNOMIAL_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COMPARE(x,y) (((x) < (y)) : -1 : ((x) == (y) : 0 : 1 ))

typedef struct polyNode * polyPointer;
typedef struct polyNode
{
	int coef; // coefficient
	int expon; // exponent
	polyPointer link;
}polyNode;

polyPointer a,b;

void attach(float coefficient,int exponent,polyPointer * ptr);

polyPointer padd(polyPointer a,polyPointer b);

#endif
