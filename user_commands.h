#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
/*#include "Game.h"*/
#include "ILP_solver.h"

#include <string.h>

bool validate(Board B, int n, int m);

bool generate(Board B, int n, int m,  int X, int Y);

bool hint(Board B, int n, int m, int X, int Y);

bool solve_ILP(Board B, int n, int m);
