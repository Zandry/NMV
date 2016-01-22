#ifndef COMMIT_H
#define COMMIT_H

#include"version.h"
#include"list.h"

struct commit;

struct commit {
	unsigned long id;
	struct version version;
	char *comment;
	//struct commit * prev;
	//struct commit * next;
	struct list_head list;
	struct list_head major_list;
	struct list_head *major_parent;
};

struct commit *new_commit(unsigned short major, unsigned long minor, char *comment);

struct commit *add_minor_commit(struct commit *from, char *comment);

struct commit *add_major_commit(struct commit *from, char *comment);

struct commit *del_commit(struct commit *victim);

void display_commit(struct commit *from);

void display_history(struct commit *from);

void display_major_history(struct commit *from);

void infos(struct commit *from, int major, unsigned long minor);

struct commit *commitOf(struct version *version);

#endif
