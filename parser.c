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

/* method to correct the insert of x,y from 1-9 to 0-8, and check if token is a legal num*/
void put(int command[], int i, char token[]){
	if(i == 1 || i ==2){
		command[i] = atoi(token)-1;
	}
	else{
		command[i] = atoi(token);
	}
	/*this is a check for num to be 0-9, uneeded * /
	if(command[i] < 0 || command[i] > BOARDSIZE){
			command[0] = INVALID;
	}
	*/
}

void settozero(int command[]){
	int i;
	for(i=0; i<4; i++){
		command[i] = 0;
	}
}

void get_command(int command[]){ 	/* command is a 4 elemnt array*/
	int i;
	char* null_if_EOF;
	char user_input[1025]; 				/* assumed in ex3 insructions max user_input length is 1024 chars*/
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
	
	if(strcmp(token, "set") == 0){
		command[0] = SET;
		for(i=1; i<4; i++){				/*4 is command.length*/
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				put(command, i, token);
			}
			else{
				command[0] = INVALID;
				return;
			}
		}
		return;
	}
	else if(strcmp(token, "hint") == 0){
		command[0] = HINT;
		for(i=1; i<3; i++){
			
			token = strtok(NULL, delimiter); /* get next part of user_input*/
			
			if(isnum(token)){
				put(command, i, token);
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
	else if(strcmp(token, "restart") == 0){
		command[0] = RESTART;
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
