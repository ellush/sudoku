#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
/*#include "Game.h"*/
#include "LP.h"
/* #include "print.h" */
#include "Save_Load.h"

bool sol;
Board cpy_board;


/*to delete later*/
bool solve_ILP(Board B, int n, int m){	
	sol = ILP_solve(B,n,m,true);
	return sol;
}

bool solve_LP(Board B, int n, int m){	
	sol = LP_guess(B,n,m,0.001);
	return sol;
}

bool validate(Board B, int n, int m){
	printf("In Validate test\n");
	if (has_error(B,n,m)){
		printf("Validation failed: the board is erroneous\n");
	 return false;
	}
	sol = ILP_solve(B,n,m,false);

	if (!sol){
		printf("Validation failed: board is unsolvable\n");
	} else {
		printf("Validation passed: board is solvable\n");
	}
	return sol; 
}


/*copies *num* of B into cpy_board*/

void copyboard(Board B, int n, int m, Board cpy_board){
/*	cpy_board = malloc(n*m*sizeof(cell*));
	int k;
	for(k=0; k<n*m; k++){
		cpy_board[k] = malloc(n*m*sizeof(cell));
	}

	*/
	int i,j;
	for(i=0; i<(n*m); i++){
		for(j=0; j<(n*m); j++){
			cpy_board[i][j].num = B[i][j].num;
		}
	}
	/*
	draw_board(n,m,cpy_board, true);
	printf("im in copyboard after draw_board\n" );
	ILP_solve(cpy_board,n,m,true);
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
		/*test copy*/
		cpy_board = malloc(n*m*sizeof(cell*));
		int k;
		for(k=0; k<n*m; k++){
			cpy_board[k] = malloc(n*m*sizeof(cell));
		}
		copyboard(B,n,m, cpy_board);
		sol = ILP_solve(cpy_board,n,m,true);
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

void guess_hint(Board B, int n, int m, int X, int Y){
	LP_guess_hint(B,n,m,X,Y);
}

int find_empty_cells(Board B, int n, int m, int empty_cells[][2]){
	int c = 0;
	int i,j;
/*	int k,l,o;*/
	for(i = 0; i < (n*m); i++){
		for(j = 0; j < (n*m); j++){
			if(B[i][j].num == 0){
				empty_cells[c][0] = i;
				empty_cells[c][1] = j;
 				c++;
			}
		}
	}
	/*  printing the double array with index*/
	/*
	for(k = 0; k < c ; k++){
		for(l = 0; l < 2; l++){
			o=empty_cells[k][l];
			printf("empty_cells[%d][%d] = %d\n",k,l,o);
		}
	}
*/
	return c;
}

void select_n_random_cells(int empty_cells[][2], int n, int num_empty_cells[],int c, int state){
	int i,j;

/*	int k,o; */
	if (state == 1){ /*for X */
		for (i = 0; i < c; i++){
			num_empty_cells[i] = i;
		}
	} else { /*for Y*/
		for (i = 0; i < c; i++){
			empty_cells[i][0] = i;
		}
	}	
		/*************delete**************************/

		/*  printing the  array with index
	for(k = 0; k < c ; k++){
		o = num_empty_cells[k];
		printf("empty_cells[%d] = %d\n",k,o);
	}
		***************************************/

	for (j = 0; j < n; j++){
		int num = rand()%(c-j);
		/*************delete**************************/

/*		printf("\nthe random num is- %d\n",num);*/
		/***************************************/
		if (state == 1){
			int tmp = num_empty_cells[num];
			num_empty_cells[num] = num_empty_cells[c-1-j];
			num_empty_cells[c-1-j] = tmp;
 	 	} else {
 	 		int tmp = empty_cells[num][0];
			empty_cells[num][0] = empty_cells[c-1-j][0];
			empty_cells[c-1-j][0] = tmp;
 	 	}
		

	}

	/***************************************/
/* to delete later !!!!!!!!!!!!!!!!!!!
	if(state == 1){
	printf("array after X- inside select n func after select \n");
	for(k = 0; k < c ; k++){
		o = num_empty_cells[k];
		printf("%d\t",o);
	}
	printf("\n");
	} else{
	**************************************/

/***************************************/
/* to delete later !!!!!!!!!!!!!!!!!!!
	printf("array after Y- inside select n func after select \n");
	for(k = 0; k < c ; k++){
		o = empty_cells[k][0];
		printf("%d\t",o);
	}
	printf("\n");
	***************************************/


}


/*tring to find random legal value for X empty cells */
bool choose_random_legal_val(Board cpy_board,int empty_cells[][2], int num_empty_cells[], int legal_val[],int n,int m, int x,int c){
	int i, j, k;
	int row, col;
	int counter;

/*	int h,o;*/
	for (i = c-1; i >= c-x ; i--){ /*for every choosen empty cell*/
		j = num_empty_cells[i];
		row = empty_cells[j][0];
		col = empty_cells[j][1];

		/* to delete later !!!!!!!!!!!!!!!!!!!*/
	/*	printf("row %d, col %d\n", row,col); */
	/*	draw_board(n,m,cpy_board, true);*/

		counter = 0;

		for(k = 0; k < (n*m) ;k++){
			int num = rand()%(n*m-k);
		/*	printf("\nthe random num is- %d\n",num);*/
		

			/***************************************/
		/* to delete later !!!!!!!!!!!!!!!!!!!

			printf("array \n");
			for(h = 0; h < (n*m) ; h++){
			o = legal_val[h];
			printf("%d\t",o);
			}
			printf("\n");
			***************************************/
			if (!is_legal_placement(cpy_board, row, col, legal_val[num],n,m)){
				counter++;
			/*	printf("counter - %d, n*m = %d\n", counter,n*m);*/
				if (counter == n*m){
					return false;
				}
			/*	printf("num is not legal %d\n",legal_val[num]);*/
				int tmp = legal_val[num];
				legal_val[num] = legal_val[n*m-1-k];
				legal_val[n*m-1-k] = tmp;
				
			} else {
			/*	printf("num is legal %d\n",legal_val[num]); */
				cpy_board[row][col].num = legal_val[num];
				break;
			}
		}

	}
/*	printf("found legal values for the X empty cells\n");
	draw_board(n,m,cpy_board, true);*/


	return true;
}


 

void generate(Board B, int n, int m, int X, int Y){
	int c,i;
	int iter_count = 0;
/*	int k,l,o;  to delete with the print command later!!!*/
	
	int empty_cells[n*m*n*m][2];

    /*insert into empty_cells array indexs of all empty cells in the board. return c = #empty cells*/
	c = find_empty_cells(B,n,m, empty_cells);

	if (X > c){
		printf("Error:: %d is larger then numer of empty cells\n",X);	
		return;  
	} 
	int num_empty_cells[c]; 
	cpy_board = malloc(n*m*sizeof(cell*));
				int s;
				for(s=0; s<n*m; s++){
					cpy_board[s] = malloc(n*m*sizeof(cell));
				}
				copyboard(B,n,m,cpy_board);
	if (X > 0){
	
			/*create array with legal values for the size of the current board*/
			int legal_val[n*m];
			for (i = 0; i < (n*m); i++){
				legal_val[i] = i+1;
			}
			/***************************************/
			/* to delete later !!!!!!!!!!!!!!!!!!!
			printf("array with legal val- generate \n");
			for(k = 0; k < n*m ; k++){
				o = legal_val[k];
				printf("%d\t",o);
			}
			printf("\n");
			***************************************/
			do{
				/*test copy*/
				
				select_n_random_cells(empty_cells, X,num_empty_cells,c,1); /*select X random cells from all empty cells in the board */
				/***************************************/
				/* to delete later !!!!!!!!!!!!!!!!!!!
				printf("array with empty cells- generate \n");
				for(k = 0; k < c ; k++){
					o = num_empty_cells[k];
					printf("%d\t",o);
				}
				printf("\n");
				***************************************/
				if (choose_random_legal_val(cpy_board,empty_cells,num_empty_cells,legal_val,n,m,X,c)){
					if (ILP_solve(cpy_board,n,m,true)){
						break;	
					}
				}
				iter_count++;
			} while (iter_count < 1000);
	}

	printf("iter_count = %d, ILP_solve status=%d\n",iter_count,ILP_solve(cpy_board,n,m,true));
	
	if ((iter_count == 1000) || ((X == 0) && (!ILP_solve(cpy_board,n,m,true)))){
		printf("Generate failed!\n" );
		int v;
		for(v=0; v<n*m; v++){
			free(cpy_board[v]);
		}
		free(cpy_board);
		return;
	}

	/* to delete later !!!!!!!!!!!!!!!!!!!*/
/*	draw_board(n,m,cpy_board, true);*/

	/*choosing Y random cells*/
	if(Y == 0){
	/*	clearboard(B,n,m);*/
	} else if( Y == (m*n*m*n)){
		/*	draw_board(n,m,B, true);*/

		copyboard(cpy_board,n,m,B);
	/*	draw_board(n,m,B, true); */

	} else {
		select_n_random_cells(empty_cells,Y,num_empty_cells,(n*m*n*m),0);


	/*deleting all but Y cells*/
	/*	draw_board(n,m,B, true); */

		clearboard(B,n,m);	

		int col,row;
		for ( i = (n*m*n*m-1); i >= (n*m*n*m-Y); i--){
			int index = empty_cells[i][0];\
		/*	printf("im the current index %d\n",index); */ 
			row = index/(n*m);
		/*	printf("row %d\n", row); */
			col = index%(m*n);
		/*	printf("col %d\n", col); */
			B[row][col].num = cpy_board[row][col].num;
		}
				draw_board(n,m,cpy_board, true);

		/*copy cpy_board to B*/
 	}
	int v;
	for(v=0; v<n*m; v++){
		free(cpy_board[v]);
	}
	free(cpy_board);
}

   



