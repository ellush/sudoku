/*main Aux is meant to simplify main, by simplifying the program flow*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "commands_modes.h" /* line should move to interface */
#include "Commands.h"
/* //#include "Interface.h" */


void play_game(Board b, int *modep,int *np, int *mp, list *undo_lst);