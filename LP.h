#ifndef LP_H
#define LP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
#include "gurobi_c.h"
#include "Game.h"
#include "Commands.h"

bool LPILP_autofill(Board B, int n, int m);

int map_variables(Board B, int n, int m, int dof_map[]);

bool LP_has_solution(Board B, int n, int m, int dof_map[]);

bool LPILP_solver(int n, int m, int dof_map[], int dof_count, double sol[], bool ILP);

bool ILP_solver(int n, int m, int dof_map[], int dof_count, double sol[]);

bool LP_solver(int n, int m, int dof_map[], int dof_count, double sol[]);

bool ILP_solve(Board B, int n, int m, bool apply);

bool LP_guesser(Board B, int n, int m, float X);

bool LP_guess_hinter(Board B, int n, int m, int X, int Y);

#endif