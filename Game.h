#ifndef GAME_H
#define GAME_H

/*this module is meant to hold main game func*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Backtrack_Solver.h"
#include "print.h"
#include "UndoRedo.h"

/*this set func recives parameters *as given by the user*, corrects them to 1- based,
this func does not print Board*/
int set(Board B, int row, int col, int val, int n,int  m, list *lst);

/*this autofill func uses 3 stacks and the set func*/
void autofill(Board B, int n, int m, list *lst, bool verbose);

/*func checks if there exists a wrong cell*/
bool has_error(Board B,int n,int m);

void mark_wrong_cells(Board B, int n, int m);

bool has_single_val(Board B,int row,int col,int *val, int n, int m);


#endif