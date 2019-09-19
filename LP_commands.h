#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
/*#include "Game.h"*/
#include "LP.h"
/* #include "print.h" */
#include "Save_Load.h"

bool ILP_validate(Board B, int n, int m);

void ILP_hint(Board B, int n, int m, int X, int Y);

bool LP_guess(Board B, int n, int m, float X, list *lst);

bool LP_guess_hint(Board B, int n, int m, int X, int Y);

bool solve_ILP(Board B, int n, int m);

void copyboard(Board B, int n, int m, Board cpy_board);

void ILP_generate(Board B, int n, int m,  int X, int Y, list *lst);