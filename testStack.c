#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "Stack.c" 


int main(int argc, char *argv[]){
	int i=7;
	int data;
	stack s;
	initialize_stack(&s);
	
	printf("enter data:");
	while(i>0){
		if(scanf("%d", &data)<=0){
			printf("An error occurred!\n");
			return -1;
		}
		push(&s, data);
		i--;
	}
	printf("\n");
	while(!isEmpty(&s)){
		data = pop(&s);
		printf("%d ", data);
	}
	
	return 1;
}

