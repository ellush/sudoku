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

void exitgame();

void clearboard(cell B[BOARDSIZE][BOARDSIZE]);

/*copies B into tmp*/
void copyboard(cell tmp[BOARDSIZE][BOARDSIZE], cell B[BOARDSIZE][BOARDSIZE]);

void draw_line();

void draw_board(cell B[BOARDSIZE][BOARDSIZE]);

void invalid();

bool is_legal_placement(cell B[BOARDSIZE][BOARDSIZE],int placement_row, int placement_col, int val);

bool game_over(cell B[BOARDSIZE][BOARDSIZE]);

void set(cell B[BOARDSIZE][BOARDSIZE], int row, int col, int val);

void hint(cell solved_B[BOARDSIZE][BOARDSIZE], int row, int col);