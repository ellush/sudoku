#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "print.c" 


bool mark_errors = true;

int main(int argc, char *argv[]){
	int i;
	int n,m;
	bool in_Edit_mode = false;
	
	Board B;
	
	printf("enter n &m");
	if(scanf("%d", &n)<=0){
		printf("An error occurred!\n");
		return -1;
	}
	if(scanf("%d", &m)<=0){
		printf("An error occurred!\n");
		return -1;
	}
	B = malloc(n*m*sizeof(cell*));
	for(i=0; i<n*m; i++){
		B[i] = malloc(n*m*sizeof(cell));
	}
	printf(" n = %d, m = %d\n", n, m);
	clearboard(n, m, B);
	draw_board(n,m,B, in_Edit_mode);
	
	
	for(i=0; i<n*m; i++){
		free(B[i]);
	}
	free(B);
	return 1;
}

