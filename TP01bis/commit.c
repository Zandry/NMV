#include<stdlib.h>
#include<stdio.h>

#include"commit.h"

static int nextId = 0;

/**
* new_commit - alloue et initialise une structure commit correspondant au parametre
* @major:      numero de version majeure
* @minor:      numero de version mineure
* @comment:    pointeur vers une chaine de caracteres contenant un commentaire
*/
struct commit *new_commit(unsigned short major, unsigned long minor, char *comment)
{
	/* TODO : Exercice 3 - Question 2 */
	struct commit *my_commit;

	my_commit = (struct commit*)malloc(sizeof(struct commit));
	my_commit->id = nextId++;
	(my_commit->version).major = major;
	(my_commit->version).minor = minor;
	(my_commit->comment) = comment;
	INIT_LIST_HEAD(&(my_commit->list));
	INIT_LIST_HEAD(&(my_commit->major_list));
	my_commit->major_parent = &(my_commit->major_list);
	/*(my_commit->next) = NULL;
	(my_commit->prev) = NULL;*/

	return my_commit;
}

/**
* insert_commit - insert sans le modifier un commit dans la liste doublement chainee
* @from:       commit qui deviendra le predecesseur du commit insere
* @new:        commit a inserer - seul ses champs next et prev seront modifie
*/
static struct commit *insert_commit(struct commit *from, struct commit *new)
{
	/* TODO : Exercice 3 - Question 3 */
	/*struct commit *temp;
	temp = last->next;
	last->next = new;
	new->prev = last;
	new->next = temp;*/
	list_add(&(new->list), &(from->list));
	return new;
}

/**
* add_minor_commit - genere et insert un commit correspondant a une version mineure
* @from:           commit qui deviendra le predecesseur du commit insere
* @comment:        commentaire du commit
*/
struct commit *add_minor_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
	struct commit *my_new_commit = new_commit((from->version).major, (from->version).minor + 1, comment);
	my_new_commit->major_parent = from->major_parent;
	/*(my_new_commit->display) = &(display_commit);
	(my_new_commit->extract) = &(extract_minor_commit);*/
	(my_new_commit->ops).display = &(display_commit);
	(my_new_commit->ops).extract = &(extract_minor_commit);
	struct commit *inserted = insert_commit(from, my_new_commit);
	return inserted;
}

/**
* add_major_commit - genere et insert un commit correspondant a une version majeure
* @from:           commit qui deviendra le predecesseur du commit insere
* @comment:        commentaire du commit
*/
struct commit *add_major_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
	struct commit *my_new_commit = new_commit((from->version).major + 1, 0, comment);
	my_new_commit->major_parent = &(my_new_commit->major_list);
	list_add(&(my_new_commit->major_list), (from->major_parent));
	/*(my_new_commit->display) = &(display_major_commit);
	(my_new_commit->extract) = &(extract_major_commit);*/
	(my_new_commit->ops).display = &(display_major_commit);
	(my_new_commit->ops).extract = &(extract_major_commit);
	struct commit *inserted = insert_commit(from, my_new_commit);
	return inserted;
}

/**
* del_commit - extrait le commit de l'historique
* @victim:         commit qui sera sorti de la liste doublement chainee
*/
struct commit *del_commit(struct commit *victim)
{
	/* TODO : Exercice 3 - Question 5 */
	/*victim->prev->next = victim->next;
	victim->next->prev = victim->prev;*/
	list_del(&(victim->list));
	free(victim);
	return NULL;
}

struct commit *extract_minor_commit(struct commit *victim){
	return del_commit(victim);
}

struct commit *extract_major_commit(struct commit *victim){
	struct commit *c;
	struct list_head *pos, *tmp;

	list_for_each_safe(pos, tmp, &(victim->list))
	/*for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)*/
	{
		c = list_entry(pos, struct commit, list);
		if (c->major_parent == victim->major_parent)
			(c->ops).extract(c);
	}
	return del_commit(victim);
}


/**
* display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
* @c:             commit qui sera affiche
*/
void display_commit(struct commit *c)
{
	/* TODO : Exercice 3 - Question 4 */
	printf("%d: ", c->id);
	display_version(&(c->version), isUnstableBis);
	printf("\t'%s'\n", c->comment);
}

/**
* display_commit - affiche un commit : "###2:  0-2 (stable) 'Work 2'###"
* @c:             commit qui sera affiche
*/
void display_major_commit(struct commit *c)
{
	if (c != NULL)
	{
		printf("%d:  ", c->id);
		printf("### version %d : ", (c->version).major);
		printf("'%s' ###\n", c->comment);
	}
}


/**
* display_history - affiche tout l'historique, i.e. l'ensemble des commits de la liste
* @from:           premier commit de l'affichage
*/
void display_history(struct commit *from)
{
	/* TODO : Exercice 3 - Question 4 */
	/*struct commit next;
	next = from;
	while(next != NULL){
	display_commit(next);
	next=next->next;
	}*/
	struct list_head *pos;
	/*display_commit(from);*/
	if ((from->version).minor == 0){
		display_major_commit(from);
	}
	else
		display_commit(from);

	struct commit *c;

	list_for_each(pos, &(from->list)) /* for (pos = (head)->next; pos != (head); pos = pos->next) */
	{
		/*display_commit(list_entry(pos, struct commit, list));*/
		c = list_entry(pos, struct commit, list);
		(c->ops).display(c);

	}
	printf("\n");
}

/**
* infos - affiche le commit qui a pour numero de version <major>-<minor>
* @major: major du commit affiche
* @minor: minor du commit affiche
*/
void infos(struct commit *from, int major, unsigned long minor){
	/* TODO : Exercice 3 - Question 5 */
	/*struct commit *ptr;
	ptr = from;
	while(ptr != NULL){
	if((ptr->version).major == major && (ptr->version).minor == minor){
	display_commit(ptr);
	return;
	}
	}*/
	struct list_head *pos;
	struct commit *temp;

	/*if((from->version).major == major && (from->version).minor == minor){
	display_commit(from);
	return;
	}

	list_for_each(pos, &(from->list))
	{
	temp = list_entry(pos, struct commit, list);
	if((temp->version).major == major && (temp->version).minor == minor){
	display_commit(temp);
	return;}
	}*/

	struct list_head *posMajor;
	struct commit *tempMajor;

	list_for_each(posMajor, &(from->major_list)){
		tempMajor = list_entry(posMajor, struct commit, major_list);
		//display_commit(tempMajor);
		if ((tempMajor->version).major == major && (tempMajor->version).minor == minor){
			(tempMajor->ops).display(tempMajor);
			// display_commit(tempMajor);
			return;
		}
		if ((tempMajor->version).major == major){
			list_for_each(pos, &(tempMajor->list)){
				temp = list_entry(pos, struct commit, list);
				if ((temp->version).major == major && (temp->version).minor == minor){
					(temp->ops).display(temp);
					//display_commit(temp);
					return;
				}
			}
		}
	}
	printf("Not here!!!\n");
}

/**
* commitOf - retourne le commit qui contient la version passe en parametre
* @version:  pointeur vers la structure version dont cherche le commit
* Note:      cette fonction continue de fonctionner meme si l'on modifie
*            l'ordre et le nombre des champs de la structure commit.
*/
struct commit *commitOf(struct version *version)
{
	/* TODO : Exercice 2 - Question 2 */
	struct commit c;
	struct commit *res = (struct commit*)((char*)version - ((char*)&(c.version) - (char*)&c));
	return NULL;
}

void freeHistory(struct commit *from){
	struct commit *ptr = from;
	struct list_head *pos, *tmp;
	/* #define list_for_each_safe(pos, n, head) 
			for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next) */
	list_for_each_safe(pos, tmp, &(from->list))
	{
		free(list_entry(pos, struct commit, list));
	}
	free(from);
}