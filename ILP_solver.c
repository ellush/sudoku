#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Game_Board.h"
#include "gurobi_c.h"
#include <string.h>

/* Add autofill */
bool ILP_solver(Board B, int n, int m, bool apply, bool ILP){
	
	int BOARDSIZE = n*m;	
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;		
	int       ind[BOARDSIZE];
	double    val[BOARDSIZE];
	double    *obj = NULL;
	double    *ub = NULL;
	char      *vtype = NULL;	
	int       optimstatus;
	double    objval;
	int       i, j, k, v, ig, jg, count;
	int       error = 0;
	double    *sol = NULL;	
	double    dof_map[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	int       dof_count = 0;
	bool      success = false;

	/* Initialize array to map and distinguish between redundant and required variables. */
	for (i = 0; i < BOARDSIZE*BOARDSIZE*BOARDSIZE; i++) {
		dof_map[i]=-1;
	}

	/*  Mark redundant variables */	
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
						for (ig = (i/m)*m ; ig < (i/m +1)*m ; ig++) {              
							for (jg = (j/n)*n ; jg < (j/n +1)*n ; jg++) { 
								dof_map[ig*BOARDSIZE*BOARDSIZE+jg*BOARDSIZE+v] = 0;
							}
						}
					}
				}         
			}
		}
	}

	/* Index the variables solved for */
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			for (v = 0; v < BOARDSIZE; v++) {
				if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]<0) {					
					dof_count++;
					dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = dof_count;                   
				}
			}
		}
	}
	
	/* Create environment */
	error = GRBloadenv(&env, NULL);
	if (error) goto QUIT;

	/* Create new model */
	if (ILP){
		vtype = (char*)malloc(dof_count * sizeof(char));
		for (i = 0; i < dof_count; i++)
			vtype[i] = GRB_BINARY;

		/*  No lb,ub needed as only unconstrained binary variables are solved for */
		error = GRBnewmodel(env, &model, "sudoku ILP", dof_count, NULL, NULL, NULL, vtype, NULL);
		free(vtype);
	} else {
		obj = (double*)malloc(dof_count * sizeof(double));
		ub  = (double*)malloc(dof_count * sizeof(double));
		for (i = 0; i < dof_count; i++)
			ub[i] = 1.0;
		error = GRBnewmodel(env, &model, "sudoku LP ", dof_count, NULL, NULL, ub, NULL, NULL);
		free(obj);
		free(ub);
	}
	if (error) goto QUIT;

	/* Apply Constrains: */

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
		for (ig = 0; ig < n; ig++) {
			for (jg = 0; jg < m; jg++) {        
				count = 0;
				for (i = ig*m; i < (ig+1)*m; i++) {
					for (j = jg*n; j < (jg+1)*n; j++) {
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
	/* This routine performs the optimization and populates internal model attributes:
	optimization status, solution, etc. */
	error = GRBoptimize(model);
	if (error) goto QUIT;

	/* Get the optimization status */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;		

	/* Process the optimization results */
	if (optimstatus == GRB_OPTIMAL){

		/* Get the value of the objective function for the computed solution */
		/* Returns a non-zero error result if no solution was found */
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) goto QUIT;

		/* Allocate memory for the solution vector: */
		sol = (double*)malloc(dof_count * sizeof(double));
		
		/* If here -> The optimization was successful */

		/* Read the solution - the assignment to each variable */
		/* dof_count id the number of variables, the size of "sol" should match */
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, dof_count, sol);
		if (error) goto QUIT;

	/*  printf("Optimal objective: %.4e\n", objval); */		
		/* printf("\n~~~~ Begining ILP Solver Debug ~~~~~:\n");
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
								printf("%d ", v+1);
							}
					}
				else
				printf("%d ", B[i][j].num );
			}
			printf("\n");
		}
		printf("\n~~~~ End of ILP Solver Debug ~~~~~:\n"); */

		
		/* Apply solution to board */
		if (apply){
			for (i = 0; i < BOARDSIZE; i++) {
				for (j = 0; j < BOARDSIZE; j++) {
					if (B[i][j].num-1 < 0)
						for (v = 0; v < BOARDSIZE; v++) {
							if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v])          
								if (sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ]>0) {		
									printf("sol:%f\n",sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ]);											
									B[i][j].num = v+1;								
								}
						}				
				}		
			}
		}

		/* Write model solution' */
		GRBupdatemodel(model);
		error = GRBwrite(model, "sudoku.sol");

		success = true;

	} else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE || optimstatus == GRB_UNBOUNDED)
		printf("LP (Gurobi) failed: Model is infeasible or unbounded (Code: %d)\n",optimstatus);
	else
		printf("LP (Gurobi) failed: Optimization failed (Code: %d)\n",optimstatus);	

	QUIT:
		
	/* Fatal error reporting */
	if (error) {
		printf("LP (Gurobi) fatal ERROR: %s\n", GRBgeterrormsg(env));	
	}	

	/* Free memory solution vectors */	
	free(sol);
	
	/* Free model */
	if ( model != NULL )
		GRBfreemodel(model);

	/* Free environment */
	if ( env != NULL )
		GRBfreeenv(env);	
	
	/* success is true only if optimization succeeded AND solution was successfully read */
	if (success)
		return true;
	else
		return false;
}