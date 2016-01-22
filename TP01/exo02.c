#include<stdlib.h>
#include<stdio.h>

#include"version.h"

struct commit *commitOf(struct version *version);

int main(int argc, char const *argv[])
{
	struct version v = {.major = 3,
			    .minor = 5,
			    .flags = 0};
 	
 	char com[] = "hello world"; 
 	
 	struct commit c = {
 		.id = 10,
 		.version = v,
 		.comment = com
 	};
 	
 	printf("commit: %p\n", &c);
	printf("id: %p\n", &(c.id));
	printf("version: %p\n", &(c.version));
	
	
	printf("____________\n");
	struct commit *res = commitOf(&(c.version)); 
	printf("commit: %p\n", res);

	return 0;
}

struct commit *commitOf(struct version *version){ 
	struct commit c; 
	struct commit *res = (struct commit*)((char*)version - ((char*)&(c.version) - (char*)&c));
	return res; 
}








