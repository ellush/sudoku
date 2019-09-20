/*this is the main module*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "commands_modes.h"
#include "main_Aux.h"


/*global*/
bool mark_errors = true;

int main(){
	
	Board b = NULL;
	list undo_lst;
	int mode  = INIT;
	int n,m;
	initialize_list(&undo_lst);
	srand(time(NULL));

	printf("WELCOME to our sudoku puzzle!\n");
	printf("we want to play a game...\n\n");
	do{
		
		play_game(&b,&mode,&n,&m, &undo_lst);
		
	}while(true); /*only exit through exit method */ 
	
	
	return 1;
}

