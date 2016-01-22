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
	struct commit *new_c = (struct commit *)malloc(sizeof(struct commit));
	new_c->id = nextId++;
	new_c->version.major = major;
	new_c->version.minor = minor;
	new_c->comment = comment;
	//new_c->prev = NULL;
	//new_c->next = NULL;
	INIT_LIST_HEAD(&(new_c->list));
	INIT_LIST_HEAD(&(new_c->major_list));
	new_c->major_parent = &(new_c->major_list);
	return new_c;
}

/**
  * insert_commit - insert sans le modifier un commit dans la liste doublement chainee
  * @from:       commit qui deviendra le predecesseur du commit insere
  * @new:        commit a inserer - seul ses champs next et prev seront modifie
  */
static struct commit *insert_commit(struct commit *from, struct commit *new)
{
	/* TODO : Exercice 3 - Question 3 */
	/*from->next = new;
	new->next = NULL;
	new->prev = from;*/
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
	struct commit * new = new_commit(from->version.major, from->version.minor+1, comment);
	new->major_parent = (from->major_parent);
  	return insert_commit(from, new);
}

/**
  * add_major_commit - genere et insert un commit correspondant a une version majeure
  * @from:           commit qui deviendra le predecesseur du commit insere
  * @comment:        commentaire du commit
  */
struct commit *add_major_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
	struct commit * new = new_commit(from->version.major+1, 0, comment);
	new->major_parent = &(new->major_list);
	list_add(&(new->major_list), (from->major_parent));
  	return insert_commit(from, new);
}

/**
  * del_commit - extrait le commit de l'historique
  * @victim:         commit qui sera sorti de la liste doublement chainee
  */
struct commit *del_commit(struct commit *victim)
{
	/* TODO : Exercice 3 - Question 5 */
	//victim->prev->next = victim->next;
	//victim->next->prev = victim->prev;
	list_del(&(victim->list));
  	return NULL;
}

/**
  * display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
  * @c:             commit qui sera affiche
  */
void display_commit(struct commit *c)
{
	/* TODO : Exercice 3 - Question 4 */
	if (c != NULL)
	{
		printf("%d:  ", c->id);
		display_version(&(c->version), is_unstableBis);
		printf("%s\n", c->comment);
	}
}

/**
  * display_history - affiche tout l'historique, i.e. l'ensemble des commits de la liste
  * @from:           premier commit de l'affichage
  */
void display_history(struct commit *from)
{
	/* TODO : Exercice 3 - Question 4 */
	// struct commit *ptr = from;
	struct list_head *pos;
	
	/*while (ptr != NULL)
	{
		display_commit(ptr);
		ptr = ptr->next;
	}*/

	list_for_each(pos, &(from->list))
	{
		display_commit(list_entry(pos, struct commit, list));
	}
	printf("\n");
}

/**
  * display_major_history - affiche l'historique majeur, i.e. l'ensemble des commits de la liste
  * @from:           premier commit de l'affichage
  */
void display_major_history(struct commit *from)
{
	/* TODO : Exercice 3 - Question 4 */
	struct list_head *pos;

	list_for_each(pos, &(from->major_list))
	{
		display_commit(list_entry(pos, struct commit, major_list));
	}
	printf("\n");
}

/**
  * infos - affiche le commit qui a pour numero de version <major>-<minor>
  * @major: major du commit affiche
  * @minor: minor du commit affiche
  */
void infos(struct commit *from, int major, unsigned long minor)
{
	/* TODO : Exercice 3 - Question 6 */
	struct commit *ptr = from;
	struct commit *ptr2 = from;
	struct list_head *pos;
	struct list_head *pos2;
	
	/*while (ptr != NULL)
	{
		if (((ptr->version).major == major) && ((ptr->version).minor == minor))
		{
			display_commit(ptr);
			return;
		}
		ptr = ptr->next;
	}*/

	/*list_for_each(pos, &(from->list))
	{
		ptr = list_entry(pos, struct commit, list);
		if (((ptr->version).major == major) && ((ptr->version).minor == minor))
		{
			display_commit(ptr);
			printf("parent release :\n");
			display_commit(list_entry(ptr->major_parent, struct commit, major_list));
			return;
		}
	}*/

	list_for_each(pos2, &(from->major_list))
	{
		ptr2 = list_entry(pos2, struct commit, major_list); // chaque major commit
		if (((ptr2->version).major == major))
		{
			list_for_each(pos, &(ptr2->list)) // parcourir chaque minor commit de major commit trouvé
			{
				ptr = list_entry(pos, struct commit, list); // un minor commit 
				if (((ptr->version).major == major) && ((ptr->version).minor == minor))
				{
					display_commit(ptr);
					return;
				}
			}
		}
	}

	printf("%2u-%lu Not Here !!!\n", major, minor);
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
  return (struct commit *)((void *)version - (void *)(&(((struct commit *)0)->version)));
}