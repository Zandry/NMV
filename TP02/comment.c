#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"comment.h"

struct comment *new_comment(
	int title_size, char *title,
	int author_size, char *author,
	int text_size, char *text)
{
	struct comment *c = (struct comment *) malloc(sizeof(struct comment));

	c->title_size = title_size;
	if (! (c->title = malloc(title_size))) {
		free(c);
		return NULL;
	}
	memcpy(c->title, title, title_size);

	c->author_size = author_size;
	if (! (c->author = malloc(author_size))) {
		free(c->title);
		free(c);
		printf("erreur\n");
		return NULL;
	}
	memcpy(c->author, author, author_size);

	c->text_size = text_size;
	if (! (c->text = malloc(text_size)))
	{
		free(c->title);
		free(c->author);
		free(c);
		return NULL;
	}
	memcpy(c->text, text, text_size);

	return c;
}

struct comment *new_comment_bis(
	int title_size, char *title,
	int author_size, char *author,
	int text_size, char *text)
{
	struct comment *c = (struct comment *) malloc(sizeof(struct comment));

	c->title_size = title_size;
	if (! (c->title = malloc(title_size))) {
		goto out_c;
	}
	memcpy(c->title, title, title_size);

	c->author_size = author_size;
	if (! (c->author = malloc(author_size))) {
		goto out_c_t;
	}
	memcpy(c->author, author, author_size);

	c->text_size = text_size;
	if (! (c->text = malloc(text_size))) {
		goto out_c_t_a;
	}
	memcpy(c->text, text, text_size);

	out: 
	return c;
	
	out_c_t_a:
		free(c->author);
	
	out_c_t:
		free(c->title);
	
	out_c:
		free(c); 
		c = NULL;
	goto out;
}

void display_comment(struct comment *c)
{
	printf("%s from %s \"%s\"\n", c->title, c->author, c->text);
}

