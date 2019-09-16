/* implemantaition of a list, ech node contains 3 integers */

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

void initialize_sub_list(sub_list *lst){
	lst->head = NULL;
	lst->cur = lst->head;
}

void sub_add(sub_list *lst, int oldval, int newval, int x, int y){
	
	sub_elem *newelem = NULL;
	newelem = (sub_elem*)malloc(sizeof(sub_elem));
	/*check if malloc failed*/
	if(newelem == NULL){
		printf("Error: malloc failed, in 'sub_add' func in sub_List module");
		printf("Exiting...\n");
		exit(1);
	}
	
	newelem->oldval = oldval;
	newelem->newval = newval;
	newelem->x = x;
	newelem->y = y;
	
	newelem->next = lst->head;
	newelem->prev = NULL;
	if(lst->head!=NULL){
		lst->head->prev = newelem;
	}
	
	lst->head = newelem;
	/*sets cur to head*/
	lst->cur = lst->head;
}

void sub_remove_head(sub_list *lst){
	sub_elem *first = NULL;
	if(lst->head == NULL){
		printf("Error! trying to remove an empty sub_List! \n");
	}
	assert(lst->head != NULL); /*trying to remove an empty sub_List!*/
		
		
	first = lst->head;
	lst->head = lst->head->next;
	if(lst->head!=NULL){
		lst->head->prev = NULL;
	}
	
	free(first);
}

bool sub_isEmpty(sub_list *lst){
	return (lst->head == NULL);
}

void print_sub_list(sub_list *lst){
	sub_elem* cur = lst->head;
	while(cur!=NULL){
		printf("Changed cell <%d,%d> from %d to %d\n", cur->x, cur->y, cur->oldval, cur->newval);
		cur = cur->next;		
	}
}

void delete_sub_list(sub_list *lst){
	sub_elem* cur = lst->head;
	sub_elem* next;
	while(cur!=NULL){
		next = cur->next;		
		free(cur);
		cur = next;
	}
	lst->head = NULL;
}
