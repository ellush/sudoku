#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "print.h"
#include "Backtrack_Solver.h"

bool mark_errors = true;

void set(Board B, int row, int col, int val){
	
	
	B[row][col].num = val;
}

int main(int argc, char *argv[]){
	int i;
	int n,m;
	bool in_Edit_mode = false;
	int row,col, val;
	
	Board B;
	
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
	clearboard(n, m, B);
	
	
	draw_board(n,m,B, in_Edit_mode);
	
	
	printf("count_sol = %d", count_solutions(B, n, m));
	
	/*
	while(val!=69){
		printf("enter row col and val, val=69 will exit\n");
		if(scanf("%d", &row)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		if(scanf("%d", &col)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		if(scanf("%d", &val)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		row--;
		col--;
		if(!is_legal_placement(B, row, col, val, n, m)){
			printf("illegal placment\n");
		}
		else{
			set(B, row, col, val);
		}
		draw_board(n,m,B, in_Edit_mode);
	}
	*/
	
	
	for(i=0; i<n*m; i++){
		free(B[i]);
	}
	free(B);
	return 1;
}

