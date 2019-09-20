/*main Aux is meant to simplify main, by simplifying the program flow*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Commands.h"
#include "Interface.h"


void play_game(Board *b, int *modep,int *np, int *mp, list *undo_lst){
	int command[4] = {0,0,0,0};
	char filepath[259]; /* max command string*/
	float f;
	get_command(command, filepath, *modep, &f);
	/*array command now holds all parameters*/
	switch(command[0]){
		case SOLVE:
			solve_command(filepath,  b, np, mp, modep,  undo_lst);
			break;
		
		case EDIT:
			edit(filepath, b, np, mp, modep,  undo_lst);
			break;
		
		case MARK_ERRORS:
			markErrors(command[1]);
			break;
			
		case PRINT_BOARD:
			printBoard(*b, *np, *mp, *modep);
			break;	
		
		case SET:
			/*revese row and col*/
			set_command(b,command[2] ,command[1], command[3], *np, *mp, modep, undo_lst);
			break;
		
		case VALIDATE:
			validate(*b, *np, *mp);
			break;
		
		case GUESS:
			guess(b, *np, *mp, modep,undo_lst, f);
			break;
		
		case GENERATE:
			generate(*b, *np, *mp, command[1],command[2], undo_lst);
			break;
		
		case UNDO:
			undo_command(*b,*np, *mp, *modep, undo_lst);
			break;
		
		case REDO:
			redo_command(*b, *np, *mp, *modep, undo_lst);
			break;
		
		case SAVE:
			save_command(filepath, *b, *np, *mp, *modep);
			break;
		
		case HINT:
			/*revese row and col*/
			hint(*b, *np, *mp, command[2], command[1]);
			break;
		
		case GUESS_HINT:
			/*revese row and col*/
			guess_hint(*b, *np, *mp, command[2],command[1]);
			break;
		
		case NUM_SOLUTIONS:
			num_solutions(*b, *np, *mp);
			break;
		
		case AUTOFILL:
			autofill_command(b, *np, *mp, modep, undo_lst);
			break;
		
		case RESET:
			reset_command(*b, *np, *mp, *modep, undo_lst);
			break;
		
		case EXIT:
			exitgame(*b, *np, *mp, undo_lst);
			break;
		
		case INVALID:
			invalid();
			break;
		
		case VOID:
			/*do nothing*/
			break;
			
		default:
			printf("Error! this is an error in get_command inside play_game func\n");
			assert(false);			
	}
}
