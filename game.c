#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define BOARDSIZE 9 /*requires BOARDSIZE to be sqr of an int*/
#define BLOCKSIZE_X 9
#define BLOCKSIZE_Y 1

#define BLOCKSIZE (int)sqrt(BOARDSIZE)
typedef struct cell{
	int num;
	bool fixed;
} cell;

void exitgame(){
	printf("Exiting...\n");
	exit(1);
}

void clearboard(cell B[BOARDSIZE][BOARDSIZE]){
	int i,j;
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			B[i][j].fixed = false;
			B[i][j].num = 0;
		}
	}
}

/*copies *num* of B into tmp*/
void copyboard(cell tmp[BOARDSIZE][BOARDSIZE], cell B[BOARDSIZE][BOARDSIZE]){
	int i,j;
	for(i=0; i<BOARDSIZE; i++){
		for(j=0; j<BOARDSIZE; j++){
			tmp[i][j].num = B[i][j].num;
		}
	}
}

void draw_line(){
	int i;
	for(i=0; i < BOARDSIZE; i++){
		printf("---");
		if (i % BLOCKSIZE_X == 0){
			printf("--");
		}
	}
	printf("-\n");
}

void draw_board(cell B[BOARDSIZE][BOARDSIZE]){
	int row;
	int col;
    
    for(row=0; row< BOARDSIZE; row++){
        if (row % BLOCKSIZE_Y == 0){
			draw_line();
		}
        for(col=0; col < BOARDSIZE; col++){
            if (col % BLOCKSIZE_X == 0){
                printf("| ");
			}
            if (B[row][col].num == 0){
                printf("   "); 
			}
            else{ 
				if(B[row][col].fixed){ 
					printf(".%d ", B[row][col].num);
				}
				else{
					printf(" %d ", B[row][col].num);
				}
			}
		}
        printf("|\n");
	}
    draw_line();
	
}

void invalid(){
	printf("Error: invalid command\n");
}

bool is_legal_placement(cell B[BOARDSIZE][BOARDSIZE],int placement_row, int placement_col, int val){
	int row, col;
		
	/* next 3 tests are double tested, this code sohuld never be reached, notice HW3 run file enables illegal placements on board* /	
	/ * val is a digit 0 - 9* /
	if(!(val >= 0 && val <= BOARDSIZE)){
		printf("val is not a legel digit\n");
		return false;
	}
	
	/ * placement_row, placement_col are legal* /
	if(!(placement_row >= 0 && placement_row <= BOARDSIZE)){
		printf("row is not in range\n");
		return false;
	}
	if(!(placement_col >= 0 && placement_col <= BOARDSIZE)){
		printf("col is not in range\n");
		return false;
	}
	/ *end of double testing*/
	
	/*place is not fixed*/
	if(B[placement_row][placement_col].fixed != false){
		printf("Error: cell is fixed\n");
		return false;
	}
	
	/*if val = 0 there are no constraints*/
	if(val == 0){
		return true;
	}
	
	/* Row constraints*/
	for(col=0; col < BOARDSIZE; col++){
		if(col != placement_col){
			if(B[placement_row][col].num == val){
				return false;
			}
		}
	}	
	
	/* Column constraints*/
    for(row=0; row < BOARDSIZE; row++){
        if(row != placement_row){
			if(B[row][placement_col].num == val){
				return false;
			}
		}
	}
	
	/* mini-square constraints*/ 
	for(row=0; row < BOARDSIZE; row++){
        for(col=0; col < BOARDSIZE; col++){
            if(col/BLOCKSIZE_X == placement_col/BLOCKSIZE_X && row/BLOCKSIZE_Y == placement_row/BLOCKSIZE_Y){   /* same mini-square*/
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

bool game_over(cell B[BOARDSIZE][BOARDSIZE]){
	int row, col;
	for(row=0; row < BOARDSIZE; row++){
		for(col=0; col < BOARDSIZE; col++){
			if(B[row][col].num == 0){
				return false;
			}
		}
	}
	printf("Puzzle solved successfully\n");
	return true;	
}

void set(cell B[BOARDSIZE][BOARDSIZE], int row, int col, int val){
	if(B[row][col].fixed == true){
		printf("Error: cell is fixed\n");
		return;
	}
	if(!is_legal_placement(B, row, col, val)){
		printf("Error: value is invalid\n");
		return;
	}
	B[row][col].num = val;
	draw_board(B);
}

void hint(cell solved_B[BOARDSIZE][BOARDSIZE], int row, int col){
	printf("Hint: set cell to %d\n", solved_B[row][col].num);
}
