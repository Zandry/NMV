#include<stdlib.h>
#include<stdio.h>

#include"version.h"

int main(int argc, char const *argv[])
{
	struct version v = {.major = 3,
			    .minor = 5,
			    .flags = 0};

	display_version(&v, is_unstableBis);
	printf("\n");

	v.minor++;
	display_version(&v, is_unstableBis);
	printf("\n");

	v.major++;
	v.minor = 0;
	display_version(&v, is_unstableBis);
	printf("\n");

	v.minor++;
	display_version(&v, is_unstableBis);
	printf("\n");

	printf("old : %d bytes\nnew : %d bytes\n", sizeof(struct version_old), sizeof(struct version));

	return 0;
}
