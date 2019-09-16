/*this module BASIC save and load func*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_board.h"

void clearboard(Board B, int n, int m);

Board makeBoard(Board B,int n, int m);

void deleteBoard(Board B, int n, int m);

/*save func, ovewrites filePath, can be changed to not destroying old file in case of error by using temp file*/
void save(char *filePath, Board b, int n, int m);

/*load func, allocates memory of Board!, old board should be freed before calling load*/
Board load(char *filePath, int *np, int *mp);