#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "Game_board.h"
/*prints board, flag in_Edit_mode to print fixed '.' or follow mark_errors*/
void draw_board(int n, int m, Board B, bool in_Edit_mode);