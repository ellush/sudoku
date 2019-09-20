/* implemantaition of a  undo/redo list, only deals with one list at a time.
last element at the llist is a FLAG, not a move */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "List.h"

void extract_changes(Board b, sub_list* lst, bool undoing, bool in_reset){
	sub_elem* cur = lst->head;
	while(cur!=NULL){
		if(undoing){
			b[cur->x][cur->y].num = cur->oldval;
			if(!in_reset){
				/*revese row and col*/
				printf("Changed cell <%d,%d> from %d to %d\n",cur->y+1, cur->x+1, cur->newval, cur->oldval);
			}		
		}
		else{
			b[cur->x][cur->y].num = cur->newval;
			/*revese row and col*/
			printf("Changed cell <%d,%d> from %d to %d\n", cur->y+1, cur->x+1, cur->oldval, cur->newval);
		}
		cur = cur->next;		
	}	
}

int undo(Board b, list *lst, bool in_reset){
	if(lst->cur->next == NULL){ /*reached flag element*/
		printf("Error: no moves to undo.\n");
		return -1;
	}
	extract_changes(b, &(lst->cur->l), true, in_reset);
	cur_next(lst);
	return 1;	
}

int redo(Board b, list *lst){
	if(lst->cur->prev == NULL){ /*reached first element*/
		printf("Error: no moves to redo.\n");
		return -1;
	}
	cur_prev(lst);
	extract_changes(b, &(lst->cur->l), false, false);
	return 1;
}

void reset(Board b, list *lst){
	while(lst->cur->next != NULL){
		undo(b, lst, true);
	}
}
