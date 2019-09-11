/* implemantaition of a list, each node contains a sub_list */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "sub_List.h"


typedef struct elem{
	sub_list l;
	struct elem *next;
	struct elem *prev;
}elem;

typedef struct list{
	elem *head;
	elem *cur;
} list;

void initialize_list(list *lst);

void cur_next(list *lst);

void cur_prev(list *lst);

/*adds an empty elem to list*/
void add_move(list *lst);

/*adds oldval,newval,x,y to the sub_list of cur*/
void add_change(list *lst, int oldval, int newval, int x, int y);

void delete_list(list *lst);

void clear_until_cur(list *lst);

void new_move(list *lst);
