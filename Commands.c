/*this module holds every command, and include all parts of execution.
 receives data unchecked! prints error and returns if needed.
 also prints board after succesful commands*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game.h"
#include "Save_Load.h"
#include "commands_modes.h"
#include "LP_commands.h"

/***************auxilary funcs for command funcs***************/
/*func checks if cell has a conflict with a fixed cell*/
bool check_fixed_conflict_cell(Board B ,int placement_row, int placement_col, int val, int n, int m){
	int row, col;
		
	/* Row constraints*/
	for(col=0; col < n*m; col++){
		if(col != placement_col){
			if(B[placement_row][col].fixed && B[placement_row][col].num == val){
				return true;
			}
		}
	}
	
	/* Column constraints*/
    for(row=0; row < n*m; row++){
        if(row != placement_row){
			if(B[row][placement_col].fixed && B[row][placement_col].num == val){
				return true;
			}
		}
	}
	
	/* block constraints*/ 
	for(row=0; row < n*m; row++){
        for(col=0; col < n*m; col++){
            if(col/n == placement_col/n && row/m == placement_row/m){   /* same block*/
				if(!(row == placement_row && col == placement_col)){    /* not the same x,y*/
					if(B[row][col].fixed && B[row][col].num == val){
						return true;
					}
				}
			}
		}
	}
	return false;	
}
/*func checks if board has two fixed cells with conflicting values*/
bool check_fixed_conflict(Board B,int n, int m){
	int i,j;
	for(i = 0; i < m*n; i++){
		for(j = 0; j < m*n; j++){
			if(B[i][j].fixed && B[i][j].wrong){
				if(check_fixed_conflict_cell(B , i, j, B[i][j].num, n, m)){
					return true;
				}
			}
		}
	}
	return false;
}

void make_no_fixed(Board b,int n,int m){
	int i,j;
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			b[i][j].fixed = false;
		}
	}		
}

void make_all_fixed(Board b,int n,int m){
	int i,j;
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(b[i][j].num != 0){
				b[i][j].fixed = true;
			}
		}
	}
}

bool has_empty_cell(Board b,int n,int m){
	int i,j;
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(b[i][j].num == 0){
				return true;
			}
		}
	}		
	return false;
}

void check_game_over(Board *b,int n,int m, int *modep, list *lst){
	if(has_empty_cell(*b,n,m)){
		return;
	}
	if(has_error(*b,n,m)){
		printf("The solution is erroneous, try again\n");
		return;
	}
	printf("The puzzle was solved successfully! good job!\n");
	*modep = INIT;
	printf("mode set to INIT\n");
	deleteBoard(*b,n,m);
	*b = NULL;
	/*restart undo list and add a flag node (first node is always a flag node)*/
	delete_list(lst);
	initialize_list(lst);
	add_move(lst);	
}

/***************command funcs***************/
void markErrors(int x){
	if(x==1){
		mark_errors = true;
		printf("mark_errors set to '1'\n");
		return;
	}
	else if(x==0){
		mark_errors = false;
		printf("mark_errors set to '0'\n");
		return;
	}
	else{
		printf("Error: must set \"mark_errors\" to either '1' or '0'\n");
	}
}

void solve_command(char *filepath, Board *b, int *np, int *mp, int *modep, list *lst){
	if(*modep != SOLVE){
		printf("mode set to SOLVE\n");
	}
	*modep = SOLVE;
	/*delete old board and list, delete empty board and list is ok*/
	deleteBoard(*b,*np, *mp);
	
	/*restart undo list and add a flag node (first node is always a flag node)*/
	delete_list(lst);
	initialize_list(lst);
	add_move(lst);	
	
	*b = load(filepath, np, mp);
	/*check for succesful load*/
	if(*b == NULL){
		printf("No board. mode set to INIT\n");
		*modep = INIT;
		*b = NULL;
		return;
	}
	
	mark_wrong_cells(*b,*np,*mp);
	
	/*check for two fixed cells with conflicting values*/
	if(check_fixed_conflict(*b, *np,*mp)){
		printf("Error: failed to load file. file contains fixed cells with conflictong values\n");
		deleteBoard(*b, *np,*mp);
		printf("No board. mode set to INIT\n");
		*modep = INIT;
		*b = NULL;
		return;
	}
	
	draw_board(*np, *mp, *b, false);
	check_game_over(b, *np, *mp, modep, lst);
}

void edit(char *filepath, Board *b, int *np, int *mp, int *modep, list *lst){
	if(*modep != EDIT){
		printf("mode set to EDIT\n");
	}
	*modep = EDIT;
	
	/*delete old board and list, delete empty board and list is ok*/
	deleteBoard(*b,*np, *mp);

	/*restart undo list and add a flag node (first node is always a flag node)*/
	delete_list(lst);
	initialize_list(lst);
	add_move(lst);	
	
	/*if no fp make a empty 9X9 Board*/
	if(strcmp(filepath, "") == 0){
		*np = 3;
		*mp = 3;
		*b = makeBoard(*b, *np,*mp);
	}
	else{
		*b = load(filepath, np, mp);
	}
	/*check for succesful load*/
	if(*b == NULL){
		printf("No board. mode set to INIT\n");
		*modep = INIT;
		*b = NULL;
		/*no need to delete board &list before return, both are deleted before intialization*/
		return;
	}
	
	make_no_fixed(*b,*np,*mp);
	mark_wrong_cells(*b,*np,*mp);
	draw_board(*np, *mp, *b, true);
}

void printBoard(Board b, int n, int m, int mode){
	if(mode == EDIT){
		draw_board(n, m, b, true); /*last bool is "in_edit_mode"*/ 
		return;
	}
	else if(mode == SOLVE){
		draw_board(n, m, b, false); /*last bool is "in_edit_mode"*/ 
		return;
	}
	else{
		printf("Error! this is an error in printBoard func. mode has undifined value.\n");
		assert(false);	
	}
}

void set_command(Board *b, int x, int y, int z,int n, int m, int *modep, list *lst){
	if(set(*b, x, y, z, n, m, lst) < 0){
		/*set failed, dont print;*/
		return;
	}
	if(*modep == SOLVE){
		draw_board(n, m, *b, false);
		check_game_over(b, n,m, modep, lst);
	}
	else if(*modep == EDIT){
		draw_board(n, m, *b, true);
	}
}

void validate(Board b, int n, int m){
	if(has_error(b,n,m)){
		printf("Error: Board is erroneous. can not validate\n");
		return;
	}
	if (ILP_validate(b,n,m)){
		printf("Validation passed: board is solvable\n");
	} else{
		printf("Validation failed: board is unsolvable\n");
	}
}

void guess(Board *b, int n, int m, int *modep, list* lst, float x){
	if((x < 0) || (x > 1)){
		printf("Error: value is not in range 0 - 1\n");
		return;
	}
	if(has_error(*b,n,m)){
		printf("Error: Board is erroneous. can not guess\n");
		return;
	}
	LP_guess(*b, n, m, x, lst);
	draw_board(n, m, *b, false);
	check_game_over(b, n,m, modep, lst);
}

void generate(Board b, int n, int m, int i, int j, list* lst){
	if((i < 0) || (i > (n*m*n*m))) {
		printf("Error: %d not in range 0 - %d\n", i,n*m*n*m);
		return;
	}
	if((j < 0) || (j > (n*m*n*m))) {
		printf("Error: %d not in range  0 - %d\n", j,n*m*n*m);
		return;
	}
	if(has_error(b,n,m)){
		printf("Error: Board is erroneous. can not generate\n");
		return;
	}
	if(ILP_generate(b,n,m,i,j,lst) > 0){;
		/*succesful*/
		draw_board(n, m, b, false);
	}
}

void undo_command(Board b,int n, int m, int mode, list* lst){
	/*check if undo failed*/
	if(undo(b, lst, false) < 0){ /*last param is bool in_reset*/
		return;
	}
	mark_wrong_cells(b,n,m);
	if(mode == SOLVE){
		draw_board(n, m, b, false);
	}
	else{
		draw_board(n, m, b, true); /*in EDIT mode*/
	}
}

void redo_command(Board b, int n, int m, int mode, list* lst){
	/*check if redo failed*/
	if(redo(b, lst) < 0){
		return;
	}
	mark_wrong_cells(b,n,m);
	if(mode == SOLVE){
		draw_board(n, m, b, false);
	}
	else{
		draw_board(n, m, b, true); /*in EDIT mode*/
	}
}

void save_command(char *filepath, Board b, int n, int m, int mode){
	if(mode == EDIT){
		if(has_error(b,n,m)){
			printf("Error: Board is erroneous. can not save\n");
			return;
		}
		/*validate*/
		if (!ILP_validate(b,n,m)){
			printf("Error: Validation failed. can not save\n");
			return;
		}
		make_all_fixed(b,n,m);
	}
	/*save file in EDIT or SOLVE mode*/
	save(filepath, b, n, m);
	/*return board to prev state*/
	if(mode == EDIT){
		make_no_fixed(b,n,m);
	}
}

void hint(Board b, int n, int m, int x, int y){
	/*user enters 1- based coor*/
	x--;
	y--;

	if(y > n*m-1 || y < 0){
		printf("Error: column coordinate is not in range 1 - %d\n",n*m);
		return;
	}
	if(x > n*m-1 || x < 0){ 
		printf("Error: row coordinate is not in range 1 - %d\n",n*m);
		return;
	}
	if(has_error(b,n,m)){
		printf("Error: Board is erroneous. can not hint\n");
		return;
	}
	
	if(b[x][y].fixed == true){
		printf("Error: cell is fixed\n");
		return;
	}
	if(b[x][y].num != 0){
		printf("Error: cell already contains a value\n");
		return;
	}
	ILP_hint(b, n, m, x, y);	
}

void guess_hint(Board b, int n, int m, int x, int y){
	/*user enters 1- based coor*/
	x--;
	y--;
	if(y > n*m-1 || y < 0){
		printf("Error: column coordinate is not in range 1 - %d\n",n*m);
		return;
	}
	if(x > n*m-1 || x < 0){ 
		printf("Error: row coordinate is not in range 1 - %d\n",n*m);
		return;
	}
	if(has_error(b,n,m)){
		printf("Error: Board is erroneous. can not guess hint\n");
		return;
	}
	
	if(b[x][y].fixed == true){
		printf("Error: cell is fixed\n");
		return;
	}
	if(b[x][y].num != 0){
		printf("Error: cell already contains a value\n");
		return;
	}
	LP_guess_hint(b, n, m, x, y);
}

void num_solutions(Board b, int n, int m){
	if(has_error(b,n,m)){
		printf("Error: Board is erroneous. can not count solutions\n");
		return;
	}
	printf("This Board has %d different solutions\n",count_solutions(b,n,m));
}

void autofill_command(Board *b, int n, int m, int *modep, list *lst){
	if(has_error(*b,n,m)){
		printf("Error: Board is erroneous. can not autofill\n");
		return;
	}
	/* autofill */
	autofill(*b, n, m, lst); 
	draw_board(n, m, *b, false);
	check_game_over(b, n,m, modep, lst);
}

void reset_command(Board b, int n, int m, int mode, list* lst){
	reset(b, lst);
	mark_wrong_cells(b,n,m);
	if(mode == SOLVE){
		draw_board(n, m, b, false);
	}
	else{
		draw_board(n, m, b, true); /*in EDIT mode*/
	}
}

void exitgame(Board b, int n, int m, list* lst){
	deleteBoard(b,n,m);
	delete_list(lst);
	printf("so long and thanks for all the fish!\nExiting...\n");
	exit(1);
}

void invalid(){
	printf("Invalid command\n");
}
