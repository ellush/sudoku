#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "commands_modes.h"

bool isnum(char token[]);

/* method to correct the insert of x,y from 1-9 to 0-8, and check if token is a legal num*/
void put(int command[], int i, char token[]);

void settozero(int command[]);

void get_command(int command[]);