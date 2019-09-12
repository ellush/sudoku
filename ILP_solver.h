#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
#include "gurobi_c.h"
#include <string.h>

bool ILP_solver(Board B, int n, int m);