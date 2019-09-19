/* implemantaition of a list, each node contains 3 integers */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "Game_board.h"


typedef struct sub_elem{
	int oldval;
	int newval;
	int x;
	int y;
	struct sub_elem *next;
	struct sub_elem *prev;
}sub_elem;

typedef struct sub_list{
	sub_elem *head;
	sub_elem *cur;
} sub_list;

void initialize_sub_list(sub_list *lst);

void sub_add(sub_list *lst, int oldval, int newval, int x, int y);

void sub_remove_head(sub_list *lst);

void print_sub_list(sub_list *lst);

void delete_sub_list(sub_list *lst);