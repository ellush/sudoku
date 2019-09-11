/*this module is meant to hold main game func*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Backtrack_Solver.h"
#include "print.h"
#include "UndoRedo.h"

/*this set func recives parameters *as given by the user*, corrects them to 1- based,
this func does not print Board*/
/*this func needs to be reveiwed after error module was written*/
int set(Board B, int row, int col, int val, int n,int  m, list *lst);

/*this autofill func uses 3 stacks and the set func*/
void autofill(Board B, int n, int m, list *lst);
