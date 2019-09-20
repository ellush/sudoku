/*this module is meant to hold main game func*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Backtrack_Solver.h"
#include "print.h"
#include "UndoRedo.h"

void mark_wrong_cells(Board B, int n, int m){
	int i,j;
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(!is_legal_placement(B, i, j, B[i][j].num, n, m)){
				B[i][j].wrong = true;
			}
			else{
				B[i][j].wrong = false;
			}
		}
	}		
}

/*this set func recives parameters *as given by the user*, corrects them to 1- based,
this func does not print Board*/
int set(Board B, int row, int col, int val, int n,int  m, list *lst){
	/*fix parameters to 0-based*/
	row--;
	col--;
	/*revese row and col*/
	if(col > n*m-1 || col < 0){
		printf("Error: column coordinate is not in range 1 - %d\n",n*m);
		return -1;
	}
	if(row > n*m-1 || row < 0){ 
		printf("Error: row coordinate is not in range 1 - %d\n",n*m);
		return -1;
	}	
	if(val > n*m || val < 0){
		printf("Error: value is not in range 0 - %d\n", n*m);
		return -1;
	}
	if(B[row][col].fixed == true){
		printf("Error: cell is fixed\n");
		return -1;
	}
	
	new_move(lst);
	/*push to undo list*/
	add_change(lst, B[row][col].num , val, row, col);
		
	/*make change*/
	B[row][col].num = val;
	
	mark_wrong_cells(B,n,m);
	return 1;
}
/*func checks if there exists a wrong cell*/
bool has_error(Board B,int n,int m){
	int i,j;
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(B[i][j].wrong){
				return true;
			}
		}
	}
	return false;	
}
/*if a cell has a single legal val - the func sets val to it & returns true*/
bool has_single_val(Board B,int row,int col,int *val, int n, int m){
	int i;
	int cnt = 0;
	for(i=1; i<=n*m; i++){
		if(is_legal_placement(B,row,col,i, n, m)){
			cnt++;
			*val=i;
		}
	}
	if(cnt==1){
		return true;
	}
	return false;
}

/*this autofill func uses 3 stacks */
void autofill(Board B, int n, int m, list *lst){
	int i,j,val;
	stack s_row, s_col, s_val;
	initialize_double_stack(&s_row,&s_col);
	initialize_stack(&s_val);
	
	/*start move*/
	new_move(lst);
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(B[i][j].num == 0){
				if(has_single_val(B,i,j,&val,n,m)){
					double_push(&s_row,i ,&s_col,j);
					push(&s_val, val);
				}
			}
		}
	}
	while(!isEmpty(&s_val)){
		double_pop(&s_row,&i,&s_col,&j);
		val = pop(&s_val);
		/*push to undo list*/
		add_change(lst, B[i][j].num , val, i, j);
		
		/*make change*/
		B[i][j].num = val;
		/*revese row and col*/
		printf("set %d in cell <%d,%d>\n",val, j+1, i+1); 
	}
	mark_wrong_cells(B,n,m);
	return;
}
