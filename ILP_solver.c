#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
#include "gurobi_c.h"
#include <string.h>

bool ILP_solver(Board B, int n, int m){
    /* These lines should be replaced */
    int BOARDSIZE = n*m;
    int BLOCKSIZE_X = n;
    int BLOCKSIZE_Y = m;
    
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;		
	int       ind[BOARDSIZE];
	double    val[BOARDSIZE];
	char      vtype[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	char     *names[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	char      namestorage[10*BOARDSIZE*BOARDSIZE*BOARDSIZE];
	char     *cursor;
	int       optimstatus;
	double    objval;
	int       i, j, k, v, ig, jg, count;
	int       error = 0;
	double    sol[BOARDSIZE*BOARDSIZE*BOARDSIZE];	
	double    dof_map[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	int       dof_count = 0;

    printf("\nHere\n");

	/* Initialize condensed array for distinguish between redundant and required variables. */
	for (i = 0; i < BOARDSIZE*BOARDSIZE*BOARDSIZE; i++) {
		dof_map[i]=-1;
	}

	/*  Create an empty model, while removing redundant variables */	
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {			
			if (B[i][j].num-1 >= 0) {				
				for (v = 0; v < BOARDSIZE; v++){
					dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = 0;
					if (B[i][j].num-1==v) {				                      
						for (k = 0; k < BOARDSIZE; k++) {
							dof_map[k*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = 0;
							dof_map[i*BOARDSIZE*BOARDSIZE+k*BOARDSIZE+v] = 0;
						}
						for (ig = (i/BLOCKSIZE_Y)*BLOCKSIZE_Y ; ig < (i/BLOCKSIZE_Y +1)*BLOCKSIZE_Y ; ig++) {              
							for (jg = (j/BLOCKSIZE_X)*BLOCKSIZE_X ; jg < (j/BLOCKSIZE_X +1)*BLOCKSIZE_X ; jg++) { 
								dof_map[ig*BOARDSIZE*BOARDSIZE+jg*BOARDSIZE+v] = 0;
							}
						}
					}
				}         
			}
		}
	}

	cursor = namestorage;
	/* Index the variables solved for */
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			for (v = 0; v < BOARDSIZE; v++) {
				if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]<0) {
					vtype[dof_count] = GRB_BINARY;
					names[dof_count] = cursor;
					sprintf(names[dof_count], "x[%d,%d,%d]", i, j, v+1);
					cursor += strlen(names[dof_count]) + 1;
					dof_count++;
					dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = dof_count;                   
				}
			}
		}
	} 


	/* Create environment */
	error = GRBloadenv(&env, "sudoku.log");
	if (error) goto QUIT;

	/* Create new model */	
	/*  No lb needed as only unconstrained variables are solved for */
	error = GRBnewmodel(env, &model, "sudoku", dof_count, NULL, NULL, NULL,
						vtype, names);
	if (error) goto QUIT;

	/* Each cell gets a value */
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
		count = 0;
		for (v = 0; v < BOARDSIZE; v++) {
			if (dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]) {
			ind[count] = dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]-1;
			val[count] = 1.0;
			count++;
			}
		}
		if (count) {
			error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL); 
			if (error) goto QUIT;
		}
		}
	}

	/* Each value must appear once in each row */
	for (v = 0; v < BOARDSIZE; v++) {
		for (j = 0; j < BOARDSIZE; j++) {
			count = 0;
			for (i = 0; i < BOARDSIZE; i++) {
				if (dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]) {
					ind[count] = dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]-1;
					val[count] = 1.0;
					count++;
				}
			}
			if (count) {
				error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL); 
				if (error) goto QUIT;
			}
		}
	}

	/* Each value must appear once in each column */
	for (v = 0; v < BOARDSIZE; v++) {
		for (i = 0; i < BOARDSIZE; i++) {
			count = 0;
			for (j = 0; j < BOARDSIZE; j++) {
				if (dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]) {
					ind[count] = dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]-1;
					val[count] = 1.0;
					count++;
				}
			}
			if (count) {
				error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL); 
				if (error) goto QUIT;
			}
		}
	}

	/* Each value must appear once in each subgrid */
	for (v = 0; v < BOARDSIZE; v++) {        
		for (ig = 0; ig < BLOCKSIZE_Y; ig++) {
			for (jg = 0; jg < BLOCKSIZE_X; jg++) {        
				count = 0;
				for (i = ig*BLOCKSIZE_Y; i < (ig+1)*BLOCKSIZE_Y; i++) {
					for (j = jg*BLOCKSIZE_X; j < (jg+1)*BLOCKSIZE_X; j++) {
						if (dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]) {
							ind[count] = dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]-1;
							val[count] = 1.0;
							count++;
						}
					}
				}
				if (count) {
					error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
					if (error) goto QUIT;
				}
			}
		}
	}
	
	/*  debug constrains */
	GRBupdatemodel(model);
	GRBwrite(model,"debug.lp");

	/* Turn off console logging */
	error = GRBsetintparam(GRBgetenv(model), "OutputFlag", 0);
	if (error) goto QUIT;

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Write model to 'sudoku.lp' */
	error = GRBwrite(model, "sudoku.lp");
	if (error) goto QUIT;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) goto QUIT;

	/* get the solution - the assignment to each variable */
	/* 3-- number of variables, the size of "sol" should match */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, dof_count, sol);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* printf("\nOptimization complete\n"); */
	if (optimstatus == GRB_OPTIMAL){
	/*  printf("Optimal objective: %.4e\n", objval); */

		printf("\n~~~~ Begining ILP Solver Debug ~~~~~:\n");

		printf("\nOriginal board:\n");
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (B[i][j].num-1 < 0)
					printf("x ");
				else
					printf("%d ", B[i][j].num );
			}
		printf("\n");
		}

		printf("\nSolution:\n");
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (B[i][j].num-1 < 0)
					for (v = 0; v < BOARDSIZE; v++) {
						if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v])          
							if (sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ]>0) {								
								B[i][j].num = v+1;
								printf("%d ", B[i][j].num);
							}
					}
				else
				printf("%d ", B[i][j].num );
			}
			printf("\n");
		}
		printf("\n");

		printf("\n~~~~ End of ILP Solver Debug ~~~~~:\n");

			
	} else if (optimstatus == GRB_INF_OR_UNBD)
		printf("Model is infeasible or unbounded\n");
	else
		printf("Optimization was stopped early\n");
	printf("\n");

	QUIT:

	/* Error reporting */
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return false;
	}

	/* Free model */
	GRBfreemodel(model);

	/* Free environment */
	GRBfreeenv(env);

	return true;
}