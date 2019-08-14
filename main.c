#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "mainAux.h" 
#define None 0


int main(int argc, char *argv[]){
	bool restart, tryagain;
	cell B[BOARDSIZE][BOARDSIZE];	
	cell solved_B[BOARDSIZE][BOARDSIZE];
	
	/*an attempt to imitate HW3, changes may be needed*/
	if(argc == 2){
		srand(atoi(argv[1]));
	}
	else{
		srand(5);
	}
	
	
	do{
		/*clear */
		restart = false;		
		clearboard(B);
		clearboard(solved_B);
		/* 	***   */
		
		/*try again until board is initialized*/
		do{
			tryagain = false;
			initialize(B, solved_B, &tryagain);
		}while(tryagain);
		
		printf("Debug: Print solved board:\n");
		draw_board(solved_B);
		draw_board(B);
		
		while(!game_over(B) && !restart){
			play_game(B, solved_B, &restart);
		}
		/*solved puzzle*/
		while(!restart){
			exit_or_restart(&restart);
		}
		
	}while(restart); /*while(true) is the same */ 
	
	
	return 1;
}

