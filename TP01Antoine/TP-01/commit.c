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
  struct commit *c = (struct commit *) malloc(sizeof(struct commit));
  c->id = nextId++;
  c->version.major = major;
  c->version.minor = minor;
  c->version.flags = 0;
  c->comment = comment;
  c->next = NULL;
  c->prev = NULL;
  return c;
}

/**
 * insert_commit - insert sans le modifier un commit dans la liste doublement chainee
 * @from:       commit qui deviendra le predecesseur du commit insere
 * @new:        commit a inserer - seul ses champs next et prev seront modifie
 */
static struct commit *insert_commit(struct commit *from, struct commit *new)
{
  if(from->next)
    from->next->prev = new;
  new->prev = from;
  new->next = from->next;
  from->next = new;

  return new;
}

/**
 * add_minor_commit - genere et insert un commit correspondant a une version mineure
 * @from:           commit qui deviendra le predecesseur du commit insere
 * @comment:        commentaire du commit
 */
struct commit *add_minor_commit(struct commit *from, char *comment)
{
  struct commit *c = new_commit(from->version.major, from->version.minor+1, comment);
  return insert_commit(from, c);
}

/**
 * add_major_commit - genere et insert un commit correspondant a une version majeure
 * @from:           commit qui deviendra le predecesseur du commit insere
 * @comment:        commentaire du commit
 */
struct commit *add_major_commit(struct commit *from, char *comment)
{
  struct commit *c = new_commit(from->version.major+1, 0, comment);
  return insert_commit(from, c);
}

/**
 * del_commit - extrait le commit de l'historique
 * @victim:         commit qui sera sorti de la liste doublement chainee
 */
struct commit *del_commit(struct commit *victim)
{
  /* TODO : Exercice 3 - Question 5 */
  return NULL;
}

/**
 * display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
 * @c:             commit qui sera affiche
 */
void display_commit(struct commit *c)
{
  printf("%lu: ", c->id);
  display_version(&(c->version), is_unstable_bis);
  printf("\t\t'%s'\n", c->comment);
}

/**
 * display_history - affiche tout l'historique, i.e. l'ensemble des commits de la liste
 * @from:           premier commit de l'affichage
 */
void display_history(struct commit *from)
{
  struct commit *ptr = from;
    
  if(from == NULL) {
    printf("NULL\n");
    return;
  }

  while(ptr->prev) ptr = ptr->prev;

  while(ptr) {
    display_commit(ptr);
    ptr = ptr->next;
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
  struct commit* ptr = from;
  while(ptr) {
    if(ptr->version.major == major && ptr->version.minor == minor) {
      display_commit(ptr);
      return;
    }
    ptr = ptr->next;
  }
  printf("%d-%lu : Not here !!!\n", major, minor);
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
  return NULL;
}
