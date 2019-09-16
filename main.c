/*this is the main module*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main_Aux.c"

/*global*/
bool mark_errors = true;

int main(){
	
	Board b = NULL;
	list undo_lst;
	int mode  = INIT;
	int n,m;
	
	do{
		
		play_game(&b,&mode,&n,&m, &undo_lst);
		
	}while(true); /*only exit through exit method */ 
	
	
	return 1;
}

