#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "List.h"

bool mark_errors = true;


int main(int argc, char *argv[]){
	int i;
	int n,m;
	bool in_Edit_mode = false;
	int row,col, val, newval;
	list l;
	initialize_list(&l);
	sub_list s;
	initialize_sub_list(&s);
	int command=9;
	
	
	while(command!=10){
		printf("printing list:\n");
		print_list(&l);
		printf("enter command\n");
		
		if(scanf("%d", &command)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		if(command==1){
			printf("enter add parmeters\n");
			scanf("%d", &val);
			scanf("%d", &newval);
			scanf("%d", &row);
			scanf("%d", &col);
			
			add_cur(&l,val,newval,row,col);
			
		}
		if(command==2){
			add(&l);
		}
		if(command==3){
			cur_prev(&l);
		}
		if(command==4){
			cur_next(&l);
			
		}
		if(command== 5){
			clear_until_cur(&l);
		}
		if(command==6){
			delete_list(&l);
			printf("list deleted\n");
		}
		
	}
	delete_list(&l);
	printf("list deleted\n");
	printf("exiting...\n");
	
	return 1;

}

