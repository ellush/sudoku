#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
#include "Game.h"
#include "LP.h"
#include "print.h" 
#include "Save_Load.h"

bool sol;
Board cpy_board;
 

/*to delete later*/
bool solve_ILP(Board B, int n, int m){	
	sol = ILP_solve(B,n,m,true);
	return sol;
}

bool solve_LP(Board B, int n, int m){	
	sol = LP_guesser(B,n,m,0.001);
	return sol;
}

bool ILP_validate(Board B, int n, int m){
	sol = ILP_solve(B,n,m,false);
	return sol; 
}


void copyboard(Board B, int n, int m, Board cpy_board){
	int i,j;
	for(i=0; i<(n*m); i++){
		for(j=0; j<(n*m); j++){
			cpy_board[i][j].num = B[i][j].num;
		}
	}
}

void ILP_hint(Board B, int n, int m, int X, int Y){
		cpy_board = makeBoard(cpy_board, n,m);
		copyboard(B,n,m, cpy_board);

		sol = ILP_solve(cpy_board,n,m,true);
		if(!sol){
			printf("Hint failed: board is unsolvable\n");	
		} else{
			printf("Hint: set cell to %d\n",cpy_board[X][Y].num);
		}
		deleteBoard(cpy_board,n ,m);
}

bool LP_guess(Board B, int n, int m, float X){
	bool s = false;
	cpy_board = makeBoard(cpy_board,n,m);
	copyboard(B,n,m,cpy_board);
	s = LP_guesser(cpy_board,n,m,X);
	if (s){
		copyboard(cpy_board, n, m, B);
	} else{
		printf("Guess failed!\n");
	}
	deleteBoard(cpy_board,n ,m);
	return s;
}

bool LP_guess_hint(Board B, int n, int m, int X, int Y){
	if (!LP_guess_hinter(B,n,m,X,Y)){
		printf("Guess_hint failed: the board is unsolvable\n");
		return false;
	}
	return true;
}


int find_empty_cells(Board B, int n, int m, int empty_cells[]){
	int c = 0;
	int i,j;
	for(i = 0; i < (n*m); i++){
		for(j = 0; j < (n*m); j++){
			if(B[i][j].num == 0){
				empty_cells[c] = (m*n)*i + j;
 				c++;
			}
		}
	}
	return c;
}

void swap( int ary[], int i, int j, int k){
    int temp = ary[i];
    ary[i] = ary[j-1-k];
    ary[j-1-k] = temp;
}

void select_n_random_cells(int n, int num_empty_cells[],int c, int state){
	int i,j;

	if (state == 0){ 
		for (i = 0; i < c; i++){
			num_empty_cells[i] = i;
		}
	}	

	for (j = 0; j < n; j++){
		int num = rand()%(c-j);
		swap(num_empty_cells, num, c,j);
	}
}

/*tring to find random legal value for X empty cells */
bool choose_random_legal_val(Board cpy_board, int num_empty_cells[], int legal_val[],int n,int m, int x,int c){
	int i, j, k;
	int row, col;
	int counter;
	for (i = c-1; i >= c-x ; i--){ /*for every choosen empty cell*/
		j = num_empty_cells[i];
		row = j/(n*m);
		col = j%(n*m);
		
		counter = 0;
		for(k = 0; k < (n*m) ;k++){
			int num = rand()%(n*m-k);
			if (!is_legal_placement(cpy_board, row, col, legal_val[num],n,m)){
				counter++;
				if (counter == n*m){
					return false;
				}
				swap(legal_val, num, n*m, k);
			} else {
				cpy_board[row][col].num = legal_val[num];
				break;
			}
		}
	}
	return true;
}



void ILP_generate(Board B, int n, int m, int X, int Y){
	int c, i, row, col, index;
	int iter_count = 0;
	int *cells_index;
	int *legal_val;

	/*allocate int[] of size c*/
	cells_index = (int*)malloc(n*m*n*m*sizeof(int));
	/*create array with legal values for the size of the current board*/
	legal_val = (int*)malloc(n*m*sizeof(int));
	cpy_board = makeBoard(cpy_board,n,m);

    /*insert into empty_cells array indexs of all empty cells in the board. return c = #empty cells*/
	c = find_empty_cells(B,n,m, cells_index);
	printf("num of empty cells %d\n",c);

	if (X > c){
		printf("Error:: %d is larger then numer of empty cells\n",X);	
		/*free resources*/
		goto END;
	} 
	if (X > 0){
		for (i = 0; i < (n*m); i++){
			legal_val[i] = i+1;
		}	
		do{
			copyboard(B,n,m,cpy_board);

			select_n_random_cells(X,cells_index,c,1); /*select X random cells from all empty cells in the board */
				
			if (choose_random_legal_val(cpy_board/*,empty_cells*/,cells_index,legal_val,n,m,X,c)){
				draw_board(n,m,cpy_board,true);/*delete later!!!!!!!!*/
				if (ILP_solve(cpy_board,n,m,true)){
					break;	
				}
			}
			iter_count++;
		} while (iter_count < 1000);
	}

	if (iter_count == 1000){
		printf("Error: generate failed!, tried 1000 iterations\n" );
		goto END;
	}
	if(X == 0 && !ILP_solve(cpy_board,n,m,true)){
		printf("Error: generate failed!\n" );
		goto END;
	}
	

	/*choosing Y random cells*/
	if(Y == 0){
		clearboard(B,n,m);
	} else if( Y == (m*n*m*n)){
		copyboard(cpy_board,n,m,B);
		/*game over?????????????*/
	} else {
		select_n_random_cells(Y,cells_index,(n*m*n*m),0);

	/*deleting all but Y cells*/
		clearboard(B,n,m);	

		for ( i = (n*m*n*m-1); i >= (n*m*n*m-Y); i--){
			index = cells_index[i];
			row = index/(n*m);
			col = index%(m*n);
			B[row][col].num = cpy_board[row][col].num;
		}
				draw_board(n,m,cpy_board, true);/*delete later!!!!!!!!!!*/
 	} 
	END:
	
	deleteBoard(cpy_board,n ,m);
	free(legal_val);
	free(cells_index);
	}



   





