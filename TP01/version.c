#include<stdlib.h>
#include<stdio.h>

#include"version.h"

int is_unstable(struct version *v)
{
	return 1 & ((char *)v)[sizeof(short)];
}

int is_unstable_bis(struct version *v)
{
	return 1 & (v->minor);
}

void display_version(struct version *v, int (*function)(struct version*))
{
	printf("%2u-%lu %s", v->major, v->minor,
			     function(v) ? "(unstable)" : "(stable)	");
}

int cmp_version(struct version *v, int major, int minor)
{
	return v->major == major && v->minor == minor;
}
