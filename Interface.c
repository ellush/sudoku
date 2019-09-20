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

void get_command(int command[], char filepath[259], int mode, float *f){ 	/* command is a 4 elemnt array*/
	int i;
	char* null_if_EOF;
	char user_input[259]; /* assumed in project insructions max user_input length is 256 chars + '\n'*/
	char* token = NULL;
	const char *delimiter = " \t\r\n";/*new line is a new command*/
	char *endptr;/*for checking strtof error*/
	settozero(command);
	
	
	printf("Please enter new command:\n");
	
	null_if_EOF = fgets(user_input, sizeof(user_input), stdin);
		
	/* get the first token */
	token = strtok(user_input, delimiter);
		
	/*check EOF*/
	if(null_if_EOF == NULL){		
		token = "exit";
	}
	
	/*check over 256 characters (+'\n' = 258)*/
	if (strlen(user_input) > 258) {
		printf("Error: entered over 256 characters\n");
		command[0] = INVALID;
		return;		
	}
	
	/*check blank line*/
	if(token == NULL){
		command[0] = VOID;
		return;
	}
	
	/*****read commands****/
	if(strcmp(token,"solve") == 0){
		command[0] = SOLVE;
		
		token = strtok(NULL, delimiter);/*read filepath*/
		if(token != NULL){	
			strcpy(filepath, token);
		}
		else{
			printf("Error: not enough parameters\n");
			command[0] = VOID;
			return;
		}
	} 
	else if(strcmp(token, "edit") == 0){
		command[0] = EDIT;
		
		token = strtok(NULL, delimiter);/*read filepath*/
		if(token != NULL){	
			strcpy(filepath, token);
		}
		else{
			/*no filepath, make an 3X3 emptyboard*/
			strcpy(filepath ,"");
			return;
		}
	}
	else if(strcmp(token, "mark_errors") == 0){
		if(mode != SOLVE){
			printf("\"mark_errors\" is not available in the current mode. try switching to SOLVE mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = MARK_ERRORS;
		token = strtok(NULL, delimiter);
		if(token == NULL){
			printf("Error: not enough parameters\n");
			command[0] = VOID;
			return;
		}
		
		if(isnum(token)){
			command[1] = atoi(token);
		}
		else{
			printf("Error: %s is not a positive integer. must set \"mark_errors\" to either '1' or '0'\n",token);
			command[0] = VOID;
			return;
		}
	}
	else if(strcmp(token, "print_board") == 0){
		if(mode == INIT){
			printf("\"print_board\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = PRINT_BOARD;
	}
	else if(strcmp(token, "set") == 0){
		if(mode == INIT){
			printf("\"set\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		
		command[0] = SET;
		for(i=1; i<4; i++){				/*4 is command.length*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			if(token == NULL){
				printf("Error: not enough parameters\n");
				command[0] = VOID;
				return;
			}
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				printf("Error: %s is not a positive integer\n",token);
				command[0] = VOID;
				return;
			}
		}
	}
	else if(strcmp(token, "validate") == 0){
		if(mode == INIT){
			printf("\"validate\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = VALIDATE;
	}
	else if(strcmp(token, "guess") == 0){
		if(mode != SOLVE){
			printf("\"guess\" is not available in the current mode. try switching to SOLVE mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = GUESS;
		token = strtok(NULL, delimiter);
		if(token == NULL){
			printf("Error: not enough parameters\n");
			command[0] = VOID;
			return;
		}
		*f = (float)strtod(token, &endptr);
		/*check if strtod failed*/
		if (*endptr != '\0'){
			printf("Error: %s is not a valid float for guess\n",token);
			command[0] = VOID;
			return;
		}
	}
	else if(strcmp(token, "generate") == 0){
		if(mode != EDIT){
			printf("\"generate\" is not available in the current mode. try switching to EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = GENERATE;
		for(i=1; i<3; i++){/*only need 2 nums*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			if(token == NULL){
				printf("Error: not enough parameters\n");
				command[0] = VOID;
				return;
			}
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				printf("Error: %s is not a positive integer\n",token);
				command[0] = VOID;
				return;
			}
		}
	}	
	else if(strcmp(token, "undo") == 0){
		if(mode == INIT){
			printf("\"undo\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = UNDO;
	}
	else if(strcmp(token, "redo") == 0){
		if(mode == INIT){
			printf("\"redo\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}	
		command[0] = REDO;
	}
	else if(strcmp(token, "save") == 0){
		if(mode == INIT){
			printf("\"save\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = SAVE;
		token = strtok(NULL, delimiter);/*read filepath*/
		if(token != NULL){	
			strcpy(filepath, token);
		}
		else{
			printf("Error: not enough parameters\n");
			command[0] = VOID;
			return;
		}
	}
	else if(strcmp(token, "hint") == 0){
		if(mode != SOLVE){
			printf("\"hint\" is not available in the current mode. try switching to SOLVE mode\n");
			command[0] = VOID;
			return;
		} 
		command[0] = HINT;
		for(i=1; i<3; i++){/*only need 2 nums*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			if(token == NULL){
				printf("Error: not enough parameters\n");
				command[0] = VOID;
				return;
			}
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				printf("Error: %s is not a positive integer\n",token);
				command[0] = VOID;
				return;
			}
		}
	}
	else if(strcmp(token, "guess_hint") == 0){
		if(mode != SOLVE){
			printf("\"guess_hint\" is not available in the current mode. try switching to SOLVE mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = GUESS_HINT;
		for(i=1; i<3; i++){/*only need 2 nums*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			if(token == NULL){
				printf("Error: not enough parameters\n");
				command[0] = VOID;
				return;
			}
			if(isnum(token)){
				command[i] = atoi(token);
			}
			else{
				printf("Error: %s is not a positive integer\n",token);
				command[0] = VOID;
				return;
			}
		}
	}
	else if(strcmp(token, "num_solutions") == 0){
		if(mode == INIT){
			printf("\"num_solutions\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = NUM_SOLUTIONS;
	}
	else if(strcmp(token, "autofill") == 0){
		if(mode != SOLVE){
			printf("\"autofill\" is not available in the current mode. try switching to SOLVE mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = AUTOFILL;
	}
	else if(strcmp(token, "reset") == 0){
		if(mode == INIT){
			printf("\"reset\" is not available in the current mode. try switching to SOLVE or EDIT mode\n");
			command[0] = VOID;
			return;
		}
		command[0] = RESET;
	}
	else if(strcmp(token, "exit") == 0){
		command[0] = EXIT;
	}
	else{
		command[0] = INVALID;
		return;
	}
	
	/* read the rest of the line*/
	token = strtok(NULL, delimiter);
	if(token != NULL){
		printf("Error: too many parameters\n");
		command[0] = VOID;
		return;
	}   
}
