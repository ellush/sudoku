#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define INVALID -1
#define SET 1
#define HINT 2
#define VALIDATE 3
#define RESTART 4
#define EXIT 5
#define BOARDSIZE 9

bool isnum(char token[]);

/* method to correct the insert of x,y from 1-9 to 0-8, and check if token is a legal num*/
void put(int command[], int i, char token[]);

void settozero(int command[]);

void get_command(int command[]);