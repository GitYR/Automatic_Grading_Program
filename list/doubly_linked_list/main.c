#include "dlist.h"

int main(void)
{
	int i,r;
	List list;

	srand(time(NULL));
	list_init(&list);

	for(i = 1; i <= 10; i++)
	{
		list_insert(&list,i);
		list_print(&list);
	}

	for(i = 1; i <= 10; i++)
	{
		list_delete(&list,i);
		list_print(&list);
	}

	for(i = 10; i >= 1; i--)
	{
		list_insert(&list,i);
		list_print(&list);
	}

	for(i = 1; i <= 10; i++)
	{
		list_delete(&list,i);
		list_print(&list);
	}

	for(i = 1; i <= 20; i++)
	{
		list_insert(&list,i);
		list_print(&list);
	}

	for(i = 1; i <= 20; i++)
	{
		r = rand() % 21;
		list_delete(&list,r);
		list_print(&list);
	}

	for(i = 1; i <= 20; i++)
	{
		list_delete(&list,i);
		list_print(&list);
	}

	for(i = 1; i <= 10; i++)
	{
		list_insert(&list,i);
		list_print(&list);
	}
	exit(EXIT_SUCCESS);
}
