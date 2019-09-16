#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "Game_board.h"

void draw_separator_row(int n, int m){
	int i;
	int numofdash = 4*m*n + m + 1; 
	for(i=0; i < numofdash ; i++){
		printf("-");
	}
	printf("\n");
}

void draw_cells_row(int n, int m, Board B, int row, bool in_Edit_mode){
	int col;
	for(col=0; col < m*n; col++){
		if (col % n == 0){
			printf("|");
		}
		printf(" ");
		if (B[row][col].num == 0){
			printf("  "); 
		}
		else{
			printf("%2d", B[row][col].num);
		}
		if(B[row][col].fixed){
			printf(".");
		}
		else if(B[row][col].wrong && (mark_errors || in_Edit_mode)){
			printf("*");
		}
		else {
			printf(" ");				
		}		
	}	
	printf("|\n");
}

void draw_board(int n, int m, Board B, bool in_Edit_mode){
	int rowofblocks;
	int rowofcells;
    
	if(B == NULL){
		return;
	}
	draw_separator_row(n,m);
    for(rowofblocks=0; rowofblocks < n; rowofblocks++){
        
        for(rowofcells=0; rowofcells < m; rowofcells++){
            draw_cells_row(n, m, B, rowofcells + rowofblocks*m, in_Edit_mode);
		}
		draw_separator_row(n,m);
	}	
}
