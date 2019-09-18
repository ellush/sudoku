#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "commands_modes.h"

bool isnum(char token[]);

void settozero(int command[]);

void get_command(int command[]/*,NULL*/);