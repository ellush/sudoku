/*main Aux is meant to simplify main, by simplifying the program flow*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Commands.h"
#include "Interface.h"
/*main game func, gets commands and calls command func accordingly*/
void play_game(Board *b, int *modep,int *np, int *mp, list *undo_lst);
