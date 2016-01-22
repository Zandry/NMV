#ifndef VERSION_H
#define VERSION_H

struct version_old {
	unsigned short major;
	unsigned long minor;
	char flags;
};

struct version {
	unsigned long minor;	
	unsigned short major;
	char flags;
};

int is_unstable(struct version *v);

int is_unstableBis(struct version *v);

void display_version(struct version *v, int (*fnc_unstable)(struct version *v));

int cmp_version(struct version *v, int major, int minor);

#endif
