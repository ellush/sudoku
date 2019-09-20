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

void initialize_list(list *lst){
	lst->head = NULL;
	lst->cur = lst->head;
}

void cur_next(list *lst){
	if(lst->cur != NULL && lst->cur->next != NULL){
		lst->cur = lst->cur->next; 
	}
}

void cur_prev(list *lst){
	if(lst->cur != NULL && lst->cur->prev != NULL){
		lst->cur = lst->cur->prev; 
	}
}

/*adds an empty elem to list*/
void add_move(list *lst){
	elem *newelem = NULL;
	newelem = (elem*)malloc(sizeof(elem));
	
	/*check if malloc failed*/
	if(newelem == NULL){
		printf("Error: malloc failed, in 'add_move' func in List module");
		printf("Exiting...\n");
		exit(1);
	}
	
	initialize_sub_list(&(newelem->l));
		
	newelem->next = lst->head;
	newelem->prev = NULL;
	if(lst->head != NULL){
		lst->head->prev = newelem;
	}
	lst->head = newelem;
	/*sets cur to head*/
	lst->cur = lst->head;
}

/*adds oldval,newval,x,y to the sub_list of cur*/
void add_change(list *lst, int oldval, int newval, int x, int y){
	if(lst->cur != NULL){
		sub_add(&(lst->cur->l), oldval, newval, x, y);
	}
	else{
		printf("cur is set to NULL, cant add to sublist");
		exit(1);
	}
}

void remove_head(list *lst){
	elem *first = NULL;	
	
	if(lst->head == NULL){
		printf("Error! trying to remove an empty sub_List! \n");
	}
	assert(lst->head != NULL); /*trying to remove an empty sub_List!*/
		
	
	first = lst->head;
	
	/*if cur is on head, move cur*/
	if(lst->cur == first){
		cur_next(lst);
	}
	
	lst->head = lst->head->next;
	
	if(lst->head!=NULL){
		lst->head->prev = NULL;
	}
	delete_sub_list(&(first->l));
	free(first);
}
/* debug func*/
void print_list(list *lst){
	elem* cur = lst->head;
	int i = 1;
	while(cur!=NULL){
		if(cur == lst->cur){
			printf("cur->");
		}
		printf("Move %d:\n", i);
		print_sub_list(&(cur->l));
		cur = cur->next;
		i++;
	}
}

void delete_list(list *lst){
	elem* cur;
	elem* next;
	
	if(lst == NULL){
		return;
	}
	cur = lst->head;	
	
	while(cur!=NULL){
		next = cur->next;
		delete_sub_list(&(cur->l));
		free(cur);
		cur = next;
	}
	lst->head = NULL;
	lst->cur = NULL;
}

void clear_until_cur(list *lst){
	elem* cur = lst->head;
	
	while(cur!=lst->cur){
		cur = cur->next;
		remove_head(lst);
	}
}

void new_move(list *lst){
	clear_until_cur(lst);
	add_move(lst);
}
