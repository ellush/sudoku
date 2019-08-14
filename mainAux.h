#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "solver.h"
#include "parser.h"

void make_cells_fixed(cell B[BOARDSIZE][BOARDSIZE], int cellstofill);

void clear_not_fixed(cell B[BOARDSIZE][BOARDSIZE]);

void initialize(cell B[BOARDSIZE][BOARDSIZE], cell solved_B[BOARDSIZE][BOARDSIZE],  bool* tryagain);
	

void exit_or_restart(bool* restart);

void play_game(cell B[BOARDSIZE][BOARDSIZE], cell solved_B[BOARDSIZE][BOARDSIZE], bool* restart);
