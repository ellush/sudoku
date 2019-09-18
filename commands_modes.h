/* this is a header file to be shared by all modules requiring 
a Board definition.
*/
#ifndef HEADER_FILE2
#define HEADER_FILE2

/*modes*/
#define INIT 1
#define SOLVE 2 /* SOLVE & EDIT are bothe modes and commands*/
#define EDIT 3
/*commands*/
#define MARK_ERRORS 4	
#define PRINT_BOARD 5		
#define SET 6
#define VALIDATE 7
#define GUESS 8
#define GENERATE 9
#define UNDO 10
#define REDO 11
#define SAVE 12
#define HINT 13
#define GUESS_HINT 14
#define NUM_SOLUTIONS 15
#define AUTOFILL 16
#define RESET 17
#define EXIT 18
#define INVALID 19
#define VOID 20

#endif