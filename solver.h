#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "gurobi_c.h"
#include "game.h"


/*randomly choose one index, then fix array*/
int pickval(int *legalval, int length);
/*finds next empty cell*/
bool find_next_cell(cell B[BOARDSIZE][BOARDSIZE], int* row, int* col);

bool solver(cell B[BOARDSIZE][BOARDSIZE]);

bool ILP_solver(cell B[BOARDSIZE][BOARDSIZE]);

bool rand_solver(cell B[BOARDSIZE][BOARDSIZE]);

void validate(cell B[BOARDSIZE][BOARDSIZE], cell solved_B[BOARDSIZE][BOARDSIZE]);