#ifndef VERSION_H
#define VERSION_H

struct version {
    unsigned long minor;
    unsigned short major;
    char flags;
};

int is_unstable(struct version *v);

int is_unstable_bis(struct version *v);

void display_version(struct version *v, int (*func)(struct version*));

int cmp_version(struct version *v, int major, int minor);

#endif
