#include <stdio.h>
#include <stdlib.h>

#include "version.h"


struct commit {
    unsigned long id;
    struct version version;
    char *comment;
};

struct commit *commitOf(struct version *v){
    return (struct commit *)((char *)v - (char *)&(((struct commit *)0)->version));
}

int main()
{

    struct version v = {.major = 3,
                        .minor = 5,
                        .flags = 0};

    char comm[] = "Slt ma biche!";
    
    struct commit c = {.id = 10,
                       .version = v,
                       .comment = comm};
   
    printf("id c'est %p\nversion c'est %p\ncomment c'est %p\n",&(c.id),&(c.version),&(c.comment));

    printf("\ncommitOf la version c'est %p\n",(commitOf(&(c.version))));

    return 0;   
}
