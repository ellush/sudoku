#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "Game.h"

bool mark_errors = true;


int main(int argc, char *argv[]){
	int i;
	int n,m;
	bool in_Edit_mode = false;
	int row,col, val;
	
	int command=9;
	
	
	Board B;
	list undo_list;
	initialize_list(&undo_list);
	/*add flag*/
	add_move(&undo_list);
	
	printf("enter n & m\n");
	if(scanf("%d", &n)<=0){
		printf("An error occurred!\n");
		return -1;
	}
	if(scanf("%d", &m)<=0){
		printf("An error occurred!\n");
		return -1;
	}
	B = malloc(n*m*sizeof(cell*));
	for(i=0; i<n*m; i++){
		B[i] = malloc(n*m*sizeof(cell));
	}
	printf(" n = %d, m = %d\n", n, m);
	clearboard(B, n, m);
	
	
	
	while(command!=10){
		printf("enter command\n");
		draw_board(n,m,B, in_Edit_mode);
		
		if(scanf("%d", &command)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		if(command==1){
			printf("enter set parmeters\n");
			scanf("%d", &row);
			scanf("%d", &col);
			scanf("%d", &val);			
			set(B,row,col,val,n,m,&undo_list);			
		}
		if(command==2){
			autofill(B, n,m,&undo_list);			
		}
		if(command==3){
			undo(B, &undo_list,false);			
		}
		if(command==4){
			redo(B, &undo_list);			
		}
		if(command==5){
			reset(B, &undo_list);			
		}
	}
	
	delete_list(&undo_list);
	for(i=0; i<n*m; i++){
		free(B[i]);
	}
	free(B);
	return 1;
}

