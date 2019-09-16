#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "Game_board.h"


/*finds next empty cell*/
bool find_next_cell(Board B,int N, int* row, int* col){
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(B[i][j].num == 0){
				*row = i;
				*col = j;
				return true;
			}
		}
	}
	return false;
}

bool is_legal_placement(Board B ,int placement_row, int placement_col, int val, int n, int m){
	int row, col;
	
	/*if val = 0 there are no constraints*/
	if(val == 0){
		return true;
	}
	
	/* Row constraints*/
	for(col=0; col < n*m; col++){
		if(col != placement_col){
			if(B[placement_row][col].num == val){
				return false;
			}
		}
	}
	
	/* Column constraints*/
    for(row=0; row < n*m; row++){
        if(row != placement_row){
			if(B[row][placement_col].num == val){
				return false;
			}
		}
	}
	
	/* block constraints*/ 
	for(row=0; row < n*m; row++){
        for(col=0; col < n*m; col++){
            if(col/n == placement_col/n && row/m == placement_row/m){   /* same block*/
				if(!(row == placement_row && col == placement_col)){    /* not the same x,y*/
					if(B[row][col].num == val){
						return false;
					}
				}
			}
		}
	}
	return true;	
}

/*increases the value of a cell until it's next legal value and returns true. 
if there are no incresed legal values for current cell, returns false*/
bool set_next_val(Board B, int n,int m, int row, int col){
	int i = B[row][col].num + 1; 
	while(i <= n*m){
		if(is_legal_placement(B, row, col, i, n ,m)){
			/*try i as value*/
			B[row][col].num = i;
			return true;
		}
			i++;
	}
	return false;
}

/*fills every cell with the first legal value it finds. if there are none or board is full, returns*/
int solve(Board B, int n, int m, stack *s_row_p, stack *s_col_p){
	int row,col;
	
	while(find_next_cell(B, n*m, &row, &col)){
		
		/*if there are no legal values, backtrack*/
		if(!set_next_val(B, n, m, row, col)){
			return 0;
		}
		
		double_push(s_row_p, row, s_col_p, col);		
	}
	
	/*no more empty cells means solved*/
	return 1;
}

/*count_solutions assumes the current board is in a correct state, meaning no error cells*/
int count_solutions(Board B, int n, int m){
	
	int row, col;
	int counter = 0;
	
	/* use two stacks as one*/
	stack s_row;
	stack s_col;
	initialize_double_stack(&s_row, &s_col);
	
	/*fill the board*/
	counter += solve(B, n, m, &s_row, &s_col);
	
	/*backtracking*/
	while(!double_isEmpty(&s_row,&s_col)){
		
		double_pop(&s_row, &row, &s_col, &col);
				
		/*backtrack until a cell has another legal value*/
		while(!set_next_val( B, n, m, row, col)){				
			/* if the stack is empty, cant backtrack, done*/
			if(double_isEmpty(&s_row,&s_col)){
				B[row][col].num = 0;/*this is new*/
				return counter;
			}
			
			B[row][col].num = 0;
			double_pop(&s_row, &row, &s_col, &col);			
		}
		/*increased a cell value, push it in the stack*/
		double_push(&s_row, row, &s_col, col);
		
		/*fill the board*/
		counter += solve(B, n, m, &s_row, &s_col);
	}
		
	return counter;
}


