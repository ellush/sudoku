#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "Game_board.h"

/*this is a temp func, should be reviwed*/
bool is_legal_placement(Board B ,int placement_row, int placement_col, int val, int n, int m);

/*count_solutions assumes the current board is in a legal state, meaning no error cells*/
int count_solutions(Board B, int n, int m);