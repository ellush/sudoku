#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "solver.h"
#include "parser.h"

void make_cells_fixed(cell B[BOARDSIZE][BOARDSIZE], int cellstofill){
	int x,y;
	
	while(cellstofill!=0){
		x = rand()%BOARDSIZE;
		y = rand()%BOARDSIZE;
		
		if(B[y][x].fixed == false){
			B[y][x].fixed = true;
			cellstofill--;
		}
	}
}

void clear_not_fixed(cell B[BOARDSIZE][BOARDSIZE]){
	int i,j;
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			if(B[i][j].fixed == false){
				B[i][j].num = 0;
			}
		}
	}
}

void initialize(cell B[BOARDSIZE][BOARDSIZE], cell solved_B[BOARDSIZE][BOARDSIZE], bool* tryagain){
	int cellstofill;
	
	printf("Please enter the number of cells to fill [0-80]:\n");
	
	if((scanf("%d", &cellstofill)<=0)){
		if(!feof(stdin)){ printf("Error: not a number\n");} /*instructions asked to write "Error: scanf has failed\n", however, HW3 doesnt write that*/
		exitgame();
		/*unreachable code*/
		printf("an error with exit func. inside intialize. this code sould have been unreachable");
		return;
	}
	
	if(cellstofill < 0 || cellstofill > 80){
		printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
		/*try again*/
		*tryagain = true;
		return;
	}
	/*create a random solution*/
	/* rand_solver(solved_B); */
	ILP_solver(solved_B);
	copyboard(B, solved_B);
	make_cells_fixed(B, cellstofill);
	clear_not_fixed(B);
	return;
}

void exit_or_restart(bool* restart){
	int command[4] = {0,0,0,0};
	
	get_command(command);
	/*array command now holds all parameters*/
	switch(command[0]){				
		case RESTART:
			*restart = true;
			break;
				
		case EXIT:
			exitgame();
			break;
				
		default:
			invalid();
	}
}

void play_game(cell B[BOARDSIZE][BOARDSIZE], cell solved_B[BOARDSIZE][BOARDSIZE], bool* restart){
	int command[4] = {0,0,0,0};
	
	get_command(command);
	/*array command now holds all parameters*/
	switch(command[0]){
		case SET:
			set(B, command[2], command[1], command[3]); /* row, col, val, not sure if correct order;*/
			break;
				
		case HINT:
			hint(solved_B, command[2], command[1]); /* again, not sure*/
			break;
				
		case VALIDATE:
			validate(B, solved_B);
			break;
				
		case RESTART:
			*restart = true;
			break;
				
		case EXIT:
			exitgame();
			break;
				
		case INVALID:
			invalid();
			break;
				
		default:
			printf("this is an error in get_command inside plat_game func\n");
			assert(false);
	}
}
