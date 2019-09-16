#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
#include "gurobi_c.h"


bool LPILP_autofill(Board B, int n, int m){
	return true;
}

int map_variables(Board B, int n, int m, int dof_map[]){
	
	int BOARDSIZE = n*m;
	int	i, j, k, v, ig, jg, dof_count = 0;

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

	return dof_count;
}

bool LPILP_solver(Board B, int n, int m, int dof_map[], int dof_count, double sol[], bool ILP){
	
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
	int       i, j, v, ig, jg, count;
	int       error = 0;	
	bool      success = false;
	int       w;
		
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

		/* Random weight */
		w = rand() % 100 + 1;
		
		ub  = (double*)malloc(dof_count * sizeof(double));
		for (i = 0; i < dof_count; i++)
			ub[i] = 1.0;

		/* set objective function */
		obj = (double*)malloc(dof_count * sizeof(double));
		/* for (i = 0; i < dof_count; i++)
			obj[i] = 0.0; */
		for (i = 0; i < dof_count; i++)
			obj[i] = rand() % (100*BOARDSIZE) + 1;

		/* Apply weights in each subgrid - bias towards each value appearing once in a subgrid */
		/* for (v = 0; v < BOARDSIZE; v++) {   */
		/* for (v = 0; v < 1; v++) {    
			for (ig = 0; ig < n; ig++) {
				for (jg = 0; jg < m; jg++) {					
					for (i = ig*m; i < ig*m+1; i++) {
						for (j = jg*n; j < jg*n+1; j++) {
							if (dof_map[i*BOARDSIZE*BOARDSIZE + j*BOARDSIZE + v]) {
								printf("Adding weight for cell <%d,%d> - value %d\n",i+1,j+1,v+1);
								obj[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ] = 1000;		
							}
						}
					}					
				}
			}
		} */

		error = GRBnewmodel(env, &model, "sudoku LP ", dof_count, obj, NULL, ub, NULL, NULL);
		
		free(ub);
		free(obj);		
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
	
	/* Turn off console logging */
	/* error = GRBsetintparam(GRBgetenv(model), "OutputFlag", 0);
	if (error) goto QUIT; */

	/* The objective is to maximize the cost function (effective only for LP) */
	error = GRBsetintattr(model, "ModelSense", -1);
	if (error) goto QUIT;

	/*  debug constrains */
	GRBupdatemodel(model);
	GRBwrite(model,"debug.lp");	

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

bool ILP_solver(Board B, int n, int m, int dof_map[], int dof_count, double sol[]){
	return LPILP_solver(B, n, m, dof_map, dof_count, sol, true);
}

bool LP_solver(Board B, int n, int m, int dof_map[], int dof_count, double sol[]){
	return LPILP_solver(B, n, m, dof_map, dof_count, sol, false);
}

bool ILP_solve(Board B, int n, int m, bool apply){
	int BOARDSIZE = n*m;
	int	dof_map[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	int i, j, v, dof_count;
	double	*sol = NULL;
	bool success;
	float score;

	/* Autofill */
	LPILP_autofill(B, n, m);	

	/* Count and create a mapping of the variables to be solved for */
	dof_count = map_variables(B, n, m, dof_map);

	/* Allocate memory for the solution vector: */
	sol = (double*)malloc(dof_count * sizeof(double));

	success = ILP_solver(B, n, m, dof_map, dof_count, sol);

	if (success && apply) {
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (B[i][j].num-1 < 0)
					for (v = 0; v < BOARDSIZE; v++) {
						if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]){
							score = sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ];
							if (score > 0) {								
								B[i][j].num = v+1;												
							}
						}							
					}
			}		
		}
	}

	/* Free memory solution vector */	
	free(sol);

	return success;
}

bool LP_guess(Board B, int n, int m, float X){
	int BOARDSIZE = n*m;
	int	dof_map[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	int i, j, v, dof_count;
	double	*sol = NULL;
	bool success;
	float score, max_score;

	/* Autofill */
	LPILP_autofill(B, n, m);
	
	/* Count and create a mapping of the variables to be solved for */
	dof_count = map_variables(B, n, m, dof_map);

	/* Allocate memory for the solution vector: */
	sol = (double*)malloc(dof_count * sizeof(double));

	success = LP_solver(B, n, m, dof_map, dof_count, sol);

	printf("Listing possible values:\n"); /* delete */

	if (success) {
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (B[i][j].num-1 < 0)
					max_score = 0;
					printf("cell <%d,%d>:",i+1,j+1); /* delete */
					for (v = 0; v < BOARDSIZE; v++) {
						if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]){
							score = sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ];
							if (score > 0)						/* delete */
								printf("\033[1;32m");			/* delete */
							printf("| %d - %2.1f ",v+1,score); 	/* delete */
							printf("\033[0m");					/* delete */
							if (score>=X) {
								/* ~TODO~ Add a check: if placement of B[i][j].num is legal */
								if (score>max_score){
									max_score = score;
									B[i][j].num = v+1;
								}															
							}
						}							
					}
					printf("\n");
			}		
		}
	}

	/* Free memory solution vector */	
	free(sol);

	return success;
}

bool LP_guess_hint(Board B, int n, int m, int X, int Y){
	int BOARDSIZE = n*m;
	int	dof_map[BOARDSIZE*BOARDSIZE*BOARDSIZE];
	int i, j, v, dof_count;
	double	*sol = NULL;
	bool success;
	float score;

	/* Check these */
	i=X-1;
	j=Y-1;

	/* Autofill */
	LPILP_autofill(B, n, m);
	
	/* Count and create a mapping of the variables to be solved for */
	dof_count = map_variables(B, n, m, dof_map);

	/* Allocate memory for the solution vector: */
	sol = (double*)malloc(dof_count * sizeof(double));
	
	success = LP_solver(B, n, m, dof_map, dof_count, sol);
	
	/* ~TODO~ ?? check: If the board is unsolvable, it is an error. */
	if (success) {		
		if (B[i][j].num-1 < 0) {
			printf("Legal values for cell <%d,%d>:\n",X,Y);
			for (v = 0; v < BOARDSIZE; v++) {				
				if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]) {
					score = sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ];
					if (score>0) {			
						printf("value: %d, score:%2.1f\n",v,score);											
						/* B[i][j].num = v+1;	 */							
					}
				}
			}
		} 
	}

	/* Free memory solution vector */	
	free(sol);

	return success;
}