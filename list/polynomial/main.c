#include "poly.h"

int main(void)
{
	ListHeader list1,list2,list3;

	init(&list1);
	init(&list2);
	init(&list3);

	insert_node_last(&list1,3,3);
	insert_node_last(&list1,2,2);
	insert_node_last(&list1,2,1);

	insert_node_last(&list2,8,3);
	insert_node_last(&list2,3,2);
	insert_node_last(&list2,10,0);

	poly_add(&list1,&list2,&list3);
	poly_print(&list3);

	poly_delete(&list1);
	poly_delete(&list2);
	poly_delete(&list3);

	return 0;
}
