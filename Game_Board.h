/* this is a header file to be shared by all modules requiring 
a Board definition.
*/
#ifndef HEADER_FILE
#define HEADER_FILE

typedef struct cell{
	int num;
	bool fixed;
	bool wrong; /*is cell erronus*/
} cell;

typedef cell** Board;
extern bool mark_errors;

#endif