#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
#include "Game.h"
#include "ILP_solver.h"


#include <string.h>
bool sol;

/*to delete later*/
bool solve_ILP(Board B, int n, int m){
	sol = ILP_solver(B,n,m,true);
	printf("%d\n",sol);
	return sol;
}

bool validate(Board B, int n, int m){
	printf("In Validate test\n");
	if (has_error(B,n,m)){
		printf("Validation failed: the board is erroneous\n");
	 return false;
	}
	sol = ILP_solver(B,n,m,false);

	if (!sol){
		printf("Validation failed: board is unsolvable\n");
	} else {
		printf("Validation passed: board is solvable\n");
	}
	return sol;
}

bool generate(Board B, int n, int m, int X, int Y);

bool hint(Board B, int n, int m, int X, int Y);


