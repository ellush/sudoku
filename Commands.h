/*this module holds every command, and include all parts of execution.
 receives data unchecked! prints error and returns if needed.
 also prints board after succesful commands*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game.h"
#include "Save_Load.h"
#include "commands_modes.h"
#include "LP_commands.h"


/***************auxilary funcs for command funcs***************/
/*func checks if cell has a conflict with a fixed cell*/
bool check_fixed_conflict_cell(Board B ,int placement_row, int placement_col, int val, int n, int m);

/*func checks if board has two fixed cells with conflicting values*/
bool check_fixed_conflict(Board B,int n, int m);

void make_no_fixed(Board b,int n,int m);

void make_all_fixed(Board b,int n,int m);

bool has_empty_cell(Board b,int n,int m);

void check_game_over(Board *b,int n,int m, int *modep, list *lst);

/***************command funcs***************/
void markErrors(int x, int *modep);

void solve_command(char *filepath, Board *b, int *np, int *mp, int *modep, list *lst);

void edit(char *filepath, Board *b, int *np, int *mp, int *modep, list *lst);

void printBoard(Board b, int n, int m, int mode);

void set_command(Board *b, int x, int y, int z,int n, int m, int *modep, list *lst);

void validate(Board b, int n, int m, int mode);

void guess(Board b, int n, int m, int mode, list* lst, float x);

void generate(Board b, int n, int m, int mode, int i, int j, list* lst);

void undo_command(Board b,int n, int m, int mode, list* lst);

void redo_command(Board b, int n, int m, int mode, list* lst);

void save_command(char *filepath, Board b, int n, int m, int mode);

void hint(Board b, int n, int m, int mode, int x, int y);

void guess_hint(Board b, int n, int m, int mode, int x, int y);

void num_solutions(Board b, int n, int m, int mode);

void autofill_command(Board *b, int n, int m, int *modep, list *lst);

void reset_command(Board b, int n, int m, int mode, list* lst);

void exitgame(Board b, int n, int m, list* lst);

void invalid();
