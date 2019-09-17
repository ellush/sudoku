/*main Aux is meant to simplify main, by simplifying the program flow*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "commands_modes.h" /*/line should move to interface*/
#include "Commands.h"
#include "parser.h"
/*#include "Interface.h"*/


void play_game(Board *b, int *modep,int *np, int *mp, list *undo_lst){
	int command[4] = {0,0,0,0};
	char *filepath = NULL; /*command string*/
	
	/*get_command(command, &filepath);*/
	/*array command now holds all parameters*/
	switch(command[0]){
		case SOLVE:
			solve_command(filepath,  b, np, mp, modep,  undo_lst);
			break;
		
		case EDIT:
			edit(filepath, b, np, mp, modep,  undo_lst);
			break;
		
		case MARK_ERRORS:
			markErrors(command[1], modep);
			break;
			
		case PRINT_BOARD:
			printBoard(*b, *np, *mp, *modep);
			break;	
		
		case SET:
			set_command(b, command[1],command[2], command[3], *np, *mp, modep, undo_lst);
			break;
		
		case VALIDATE:
			/*/command needs work;*/
			validate(*b, *np, *mp, *modep);
			break;
		
		case GUESS:
			/*/command needs work;*/
			guess(*b, *np, *mp, *modep, undo_lst);
			break;
		
		case GENERATE:
			/*/command needs work;*/
			generate(*b, *np, *mp, *modep, undo_lst);
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
			/*/command needs work;*/
			hint(*b, *np, *mp, *modep, command[1],command[2]);
			break;
		
		case GUESS_HINT:
			/*/command needs work;*/
			guess_hint(*b, *np, *mp, *modep, command[1],command[2]);
			break;
		
		case NUM_SOLUTIONS:
			num_solutions(*b, *np, *mp, *modep);
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
				
		default:
			printf("Error! this is an error in get_command inside play_game func\n");
			assert(false);			
	}
}
