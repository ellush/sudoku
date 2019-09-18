#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "commands_modes.h"



bool isnum(char token[]){
	unsigned int i;
	if(token == NULL){
		return false;
	}
	for(i=0; i < strlen(token); i++){
		if(!isdigit(token[i])){
			return false;
		}
	}	
	return true;
}

void settozero(int command[]){
	int i;
	for(i=0; i<4; i++){
		command[i] = 0;
	}
}

void get_command(int command[]/*, NULL*/){ 	/* command is a 4 elemnt array*/
	int i;
	char* null_if_EOF;
	char user_input[257]; 				/* assumed in project insructions max user_input length is 256 chars*/
	char* token;
	const char delimiter[8] = " \t\r\n";

	settozero(command);
	do{
		null_if_EOF = fgets(user_input, sizeof(user_input), stdin);
		
		/* get the first token */
		token = strtok(user_input, delimiter);
		
		/*check EOF, this is a recent change. could cause bug*/
		if(null_if_EOF == NULL){		
			token = "exit";
		}
	}while(token == NULL);
	
	if(null_if_EOF == NULL){		/*check EOF*/
		token = "exit";
	}
	

	if(strcmp(token,"solve") == 0){
		command[0] = SOLVE;
		token = strtok(NULL, delimiter);
		/*to continue...*/
		return;
	} 
	else if(strcmp(token, "edit") == 0){
		command[0] = EDIT;
		/*to continue...*/
		/*delete later! for testing only!!!!!!!*/
		for(i=1; i<3; i++){
			
			token = strtok(NULL, delimiter); 
			
			if(isnum(token)){

				command[i] = atoi(token);
			/*	printf("command[i]= %d\n", command[i]); */
			}
			else{

				command[0] = INVALID;
				return;
			}
		} 
		return;
	}
	else if(strcmp(token, "mark_errors") == 0){
		command[0] = MARK_ERRORS;
		token = strtok(NULL, delimiter);
		if(isnum(token)){
			command[1] = atoi(token);
				}
			else{
				command[0] = INVALID;
				return;
			}
		return;	
	}
	else if(strcmp(token, "print_board") == 0){
		command[0] = PRINT_BOARD;
		return;
	}
	else if(strcmp(token, "set") == 0){
		command[0] = SET;
		for(i=1; i<4; i++){				/*4 is command.length*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				command[0] = INVALID;
				return;
			}
		}

		return;
	}
	else if(strcmp(token, "validate") == 0){
		command[0] = VALIDATE;
		return;
	}
	else if(strcmp(token, "guess") == 0){
		command[0] = GUESS;
		token = strtok(NULL, delimiter);
		command[1] = 0;
		/*to continue...*/
		return;
	}
	else if(strcmp(token, "generate") == 0){
		command[0] = GENERATE;
		for(i=1; i<3; i++){
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				command[0] = INVALID;
				return;
			}
		}
		return;
	}	
	else if(strcmp(token, "undo") == 0){
		command[0] = UNDO;
		return;
	}
	else if(strcmp(token, "redo") == 0){
		command[0] = REDO;
		return;
	}
	else if(strcmp(token, "save") == 0){
		command[0] = SAVE;
		token = strtok(NULL, delimiter);
		/*to continue...*/
		return;
	}
	else if(strcmp(token, "hint") == 0){
		command[0] = HINT;
		for(i=1; i<3; i++){
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				command[0] = INVALID;
				return;
			}
		}
		return;
	}
	else if(strcmp(token, "guess_hint") == 0){
		command[0] = GUESS_HINT;
		for(i=1; i<3; i++){
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				command[0] = INVALID;
				return;
			}
		}
		return;
	}
	else if(strcmp(token, "num_solutions") == 0){
		command[0] = NUM_SOLUTIONS;
		return;
	}else if(strcmp(token, "autofill") == 0){
		command[0] = AUTOFILL;
		return;
	}
	else if(strcmp(token, "reset") == 0){
		command[0] = RESET;
		return;
	}
	else if(strcmp(token, "exit") == 0){
		command[0] = EXIT;
		return;
	}
	else{
		command[0] = INVALID;
		return;
	}
   
}
