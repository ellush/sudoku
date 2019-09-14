/*this module BASIC save and load func*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "Game_Board.h"

void clearboard(Board B, int n, int m){
	int i,j;
	for(i = 0; i < m*n; i++){
		for(j = 0; j < m*n; j++){
			B[i][j].fixed = false;
			B[i][j].wrong = false;
			B[i][j].num = 0;
		}
	}
}

Board makeBoard(Board B,int n, int m){
	int i;
	B = malloc(n*m*sizeof(cell*));
	for(i=0; i<n*m; i++){
		B[i] = malloc(n*m*sizeof(cell));
	}
	clearboard(B, n, m);
	return B;
}

void deleteBoard(Board B, int n, int m){
	int i;
	if(B == NULL){
		return;
	}
	for(i=0; i<n*m; i++){
		free(B[i]);
	}
	free(B);
}

/*save func, ovewrites filePath, can be changed to not destroying old file in case of error by using temp file*/
void save(char *filePath, Board b, int n, int m){
	FILE *fp = NULL;
	int i,j;
	
	fp = fopen(filePath, "w");
	if(fp == NULL){
		printf("Error! failed to save file. fopen returned NULL");
		return;
	}
	
	/*start writing*/
	fprintf(fp, "%d %d\n", m,n);
	for(i = 0; i < n*m; i++){
		for(j = 0; j < n*m; j++){
			fprintf(fp,"%d",b[i][j].num);
			if(b[i][j].fixed){
				fprintf(fp,".");
			}
			fprintf(fp," ");
		}
		fprintf(fp, "\n");
	}
	/*check for writing errors*/
	if(ferror(fp)){
		printf("Error! failed to save file. write error");
		return;
	}
	/*close file*/
	if(fclose(fp) == EOF){
		printf("Error! failed to save file. fclose returned EOF");
		return;
	}
}

/*load func, allocates memory of Board!, old board should be freed before calling load*/
Board load(char *filePath, int *np, int *mp){
	FILE *fp = NULL;
	int i,j;
	int n,m;
	char c;
	Board b = NULL;
	
	
	fp = fopen(filePath, "r");
	if(fp == NULL){
		printf("Error! failed to load file \"%s\". no such file!", filePath);
		return NULL;
	}
	
	/*start reading*/
	if(fscanf(fp, "%d",np) <= 0 || fscanf(fp, "%d",mp) <= 0 || *mp <= 0 || *np <= 0){
		printf("Error! failed to load file. wrong format");
		return NULL;
	}
	
	n = *np;
	m = *mp;
	
	/*allocate Board*/
	b = makeBoard(b, n, m);
	
	for(i = 0; i < n*m; i++){
		for(j = 0; j < n*m; j++){
			if(fscanf(fp,"%d",&b[i][j].num) <= 0 || b[i][j].num > n*m || b[i][j].num < 0){
				printf("Error! failed to load file. wrong format");
				deleteBoard(b,n,m);
				return NULL;
			}
			/* if we read "3    .   4" that is an error!*/
			c = getc(fp);
			if(c != ' ' && c != '.' && c != '\n' && c != EOF){
				printf("Error! failed to load file. wrong format");
				deleteBoard(b,n,m);
				return NULL;
			}
			if(c == '.'){
				b[i][j].fixed = true;
			}
		}
	}
	/*skip all spaces and check EOF*/
	do{c = getc(fp); }
	while (isspace(c));
	
	if(!feof(fp)){
		printf("Error! failed to load file. finished reading file, haven't reached EOF");
		deleteBoard(b,n,m);
		return NULL;
	}
	/*close file*/
	if(fclose(fp) == EOF){
		printf("Error! failed to load file. fclose returned EOF");
		deleteBoard(b,n,m);
		return NULL;
	}
	return b;
}