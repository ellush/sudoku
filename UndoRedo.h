/* implemantaition of a  undo/redo list, only deals with one list at a time.
last element at the llist is a FLAG, not a move */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "List.h"

int undo(Board b, list *lst, bool in_reset);

int redo(Board b, list *lst);

void reset(Board b, list *lst);