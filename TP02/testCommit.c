#include<stdlib.h>
#include<stdio.h>

#include"commit.h"

int main(int argc, char const* argv[])
{
	struct commit *first = new_commit(0, 0, "First !");
	struct commit *tmp, *victim, *last, *victim2;
	struct list_head *history = &(first->list);
	
	if (first == commitOf(&(first->version)))
		printf("OK\n");
	//printf("%d\n", &(((struct commit *)0)->version));

	display_commit(first);
	printf("\n");

	display_history(first);

	tmp = add_minor_commit(first, "Work 1");
	tmp = add_minor_commit(tmp, "Work 2");
	victim = add_minor_commit(tmp, "Work 3");
	last = add_minor_commit(victim, "Work 4");
	display_history(first);
 
	del_commit(victim);
	display_history(first);

	tmp = add_major_commit(last, "Release 1");
	victim2 = tmp;
	tmp = add_minor_commit(tmp, "Work 1");
	tmp = add_minor_commit(tmp, "Work 2");
	tmp = add_major_commit(tmp, "Release 2");
	tmp = add_minor_commit(tmp, "Work 1");
	display_history(first);

	add_minor_commit(last, "Oversight !!!");
	display_history(first);
	
	display_major_history(first);

	infos(first, 1, 2);

	infos(first, 1, 7);

	infos(first, 4, 2);

	printf("------------------\n");

	(victim2->ops).extract(victim2);

	display_history(first);

	freeHistory(first);

	return 0;
}
