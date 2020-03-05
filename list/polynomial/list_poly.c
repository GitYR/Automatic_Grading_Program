#include "list_poly.h"

void attach(float coefficient,int exponent,polyPointer * ptr)
{
	/* coef = coefficient, expon = exponent */
	/* As a matter of fact, the value 'ptr' is a double pointer */

	polyPointer temp;
	temp = (polyPointer)malloc(sizeof(*temp));
	if(temp == NULL)
	{
		fprintf(stderr,"error on allocating memory\n");
		exit(1);
	}

	temp->coef = coefficient;
	temp->expon = exponenet;
	(*ptr)->link = temp;
	*ptr = temp;
}

polyPointer padd(polyPointer a,polyPointer b)
{
	/* it returns an aggregated result between a and b */
	
	int sum;
	/* c,rear and temp is polyPointer value */
	polyPointer c,rear,temp;
	
	rear = (polyPointer)malloc(sizeof(*rear));
	c = rear; // the value 'c' points to the value 'rear'

	while(a && b)
	{
		switch(COMPARE(a->expon,b->expon))
		{
			case -1: // if b's exponent is bigger than a's
				attach(b->coef,b->expon,&rear);
				b = b->link;
				break;
			case 0: // if a's exponent is equal to b's exponent
				sum = a -> coef + b -> coef;
				if(sum) attach(sum,a->expon,&rear);
				a = a -> link; b = b -> link;
				break;
			case 1: // the exact opposite of 'case -1'
				attach(a->coef,a->expon,&rear);
				a = a->link;
				break;
		}
	}

	/* copy the rest of a and b to rear */
	for(;a;a=a->link) attach(a->coef,a->expon,&rear);
	for(;b;b=b->link) attach(b->coef,b->expon,&rear);
	rear->link = NULL;

	/* delete an useless first nodes */
	temp = c;
	c = c -> link; // "c -> link" means the rear.
	free(temp); // free c
	return c;
}
