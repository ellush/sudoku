#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
/*#include "Game.h"*/
#include "ILP_solver.h"
#include <string.h>
#include "print.h"

bool sol;
Board cpy_board;


/*to delete later*/
bool solve_ILP(Board B, int n, int m){
	sol = ILP_solver(B,n,m,true,false);
	printf("%d\n",sol);
	return sol;
}

bool validate(Board B, int n, int m){
	printf("In Validate test\n");
	if (has_error(B,n,m)){
		printf("Validation failed: the board is erroneous\n");
	 return false;
	}
	sol = ILP_solver(B,n,m,false,true);

	if (!sol){
		printf("Validation failed: board is unsolvable\n");
	} else {
		printf("Validation passed: board is solvable\n");
	}
	return sol; 
}


/*copies *num* of B into tmp
remember to free cpy_board!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

void copyboard(Board B, int n, int m){
	cpy_board = malloc(n*m*sizeof(cell*));
	int k;
	for(k=0; k<n*m; k++){
		cpy_board[k] = malloc(n*m*sizeof(cell));
	}
	int i,j;
	for(i=0; i<(n*m); i++){
		for(j=0; j<(n*m); j++){
			cpy_board[i][j].num = B[i][j].num;
		}
	}
	/*
	draw_board(n,m,cpy_board, true);
	printf("im in copyboard after draw_board\n" );
	ILP_solver(cpy_board,n,m,true);
	draw_board(n,m,cpy_board, true);
	*/
	/*
	int v;
	
	for(v=0; v<n*m; v++){
		free(cpy_board[v]);
	}
	free(cpy_board);
	*/
}


/*need to check if X,Y in the right range!!!!!!!!*/
void hint(Board B, int n, int m, int X, int Y){
	if (has_error(B,n,m)){
		printf("Hint failed: the board is erroneous\n");
		return;
	} else if(B[X][Y].fixed){
		printf("Hint failed: cell is fixed\n");
		return;
	} else if(B[X][Y].num != 0){
		printf("Hint failed: cell already contains a value\n");
		return;
	} else{
		copyboard(B,n,m);
		sol = ILP_solver(cpy_board,n,m,true,true);
		if(!sol){
			printf("Hint failed: board is unsolvable\n");	
		} else{
			printf("Hint: set cell to %d\n",cpy_board[X][Y].num);
		}

	}
	int v;
	for(v=0; v<n*m; v++){
		free(cpy_board[v]);
	}
	free(cpy_board);
}

void generate(Board B, int n, int m,  int X, int Y);




