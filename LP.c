#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Game_board.h"
#include "gurobi_c.h"
#include "Game.h"
#include "Commands.h"

/* LPILP_autfill is recursive. Returns false once no remaining cells can be auto-filled.*/
bool LPILP_autofill(Board B, int n, int m){
	
	int i,j,v;
	bool filled = false;
		
	for(i=0; i < n*m; i++){
		for(j=0; j< n*m; j++){
			if(B[i][j].num == 0){
				if(has_single_val(B,i,j,&v,n,m)){
					B[i][j].num = v;
					filled = true;				
				}
			}
		}
	}

	if ( filled )
	 	return LPILP_autofill(B, n, m);
	else
		return false;
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
					/* Reduce variables within the same cell */
					dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = 0;
					if (B[i][j].num-1==v) {
						/* Reduce variables within the whole row and column*/				                      
						for (k = 0; k < BOARDSIZE; k++) {
							dof_map[k*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] = 0;
							dof_map[i*BOARDSIZE*BOARDSIZE+k*BOARDSIZE+v] = 0;
						}
						/* Reduce variables within the subgrid containing the cell*/
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

bool LPILP_solver(int n, int m, int dof_map[], int dof_count, double sol[], bool ILP){
	
	int BOARDSIZE = n*m;	
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;		
	int       *ind = NULL;
	double    *val = NULL;
	double    *obj = NULL;
	double    *ub = NULL;
	char      *vtype = NULL;	
	int       optimstatus;
	double    objval;
	int       i, j, v, ig, jg, count;
	int       error = 0;	
	bool      success = false;
	
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
		
		ub  = (double*)malloc(dof_count * sizeof(double));
		for (i = 0; i < dof_count; i++)
			ub[i] = 1.0;

		/* set objective function */
		obj = (double*)malloc(dof_count * sizeof(double));

		/* Randomize weights */
		for (i = 0; i < dof_count; i++)
			obj[i] = rand() % (3*BOARDSIZE) + 1;

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

	/* Create arrays for indexing the constrains (max size - number of dofs) */
	ind  =    (int*)malloc(dof_count * sizeof(int));
	val  = (double*)malloc(dof_count * sizeof(double));

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

	/* Free constrains indexing arrays */
	free(ind);
	free(val);
	
	/* Turn off console logging */
	error = GRBsetintparam(GRBgetenv(model), "OutputFlag", 0);
	if (error) goto QUIT;

	/* The objective is to maximize the cost function (effective only for LP) */
	error = GRBsetintattr(model, "ModelSense", -1);
	if (error) goto QUIT;

	/*  debug constrains */
	error = GRBupdatemodel(model);
	if (error) goto QUIT;
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

bool ILP_solver(int n, int m, int dof_map[], int dof_count, double sol[]){
	if ( dof_count>0 )
		return LPILP_solver(n, m, dof_map, dof_count, sol, true);
	else
		return false;
}

bool LP_solver(int n, int m, int dof_map[], int dof_count, double sol[]){
	if ( dof_count>0 )
		return LPILP_solver(n, m, dof_map, dof_count, sol, false);
	else
		return false;
}

bool ILP_solve(Board B, int n, int m, bool apply){
	int BOARDSIZE = n*m;
	int	*dof_map = NULL;
	int i, j, v, dof_count;
	double	*sol = NULL;
	bool success;
	float score;

	/* Assumption: The board has no errors at this stage. Verified by the calling function. */
	
	/* Autofill */
	LPILP_autofill(B, n, m);	
	
	if ( !has_empty_cell(B,n,m) ) {
		success = true; /* Board is full and valid */
	} else {
		/* Run solver for partially empty and valid board:*/
			
		/* Count and create a mapping of the variables to be solved for */
		dof_map = (int*)malloc(BOARDSIZE*BOARDSIZE*BOARDSIZE * sizeof(int));
		dof_count = map_variables(B, n, m, dof_map);

		/* Allocate memory for the solution vector: */
		sol = (double*)malloc(dof_count * sizeof(double));

		success = ILP_solver(n, m, dof_map, dof_count, sol);	

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
	}

	/* Free memory solution vector */	
	free(sol);

	return success;
}

bool LP_guesser(Board B, int n, int m, float X){
	int BOARDSIZE = n*m;
	int	*dof_map = NULL;
	int i, j, k, v, dof_count, num_choices, rnd, choice;
	float score, sum_of_weights;
	double	*sol = NULL;	
	float *scores = NULL;
	bool success;

	/* Assumption: The board has no errors at this stage. Verified by the calling function. */

	/* Autofill */
	LPILP_autofill(B, n, m);

	if ( !has_empty_cell(B,n,m) ) {
		success = true; /* Board is full and valid */
	} else {

		/* Run solver for partially empty and valid board:*/

		/* Count and create a mapping of the variables to be solved for */
		dof_map = (int*)malloc(BOARDSIZE*BOARDSIZE*BOARDSIZE * sizeof(int));
		dof_count = map_variables(B, n, m, dof_map);

		/* Allocate memory for the solution vector: */
		sol = (double*)malloc(dof_count * sizeof(double));

		success = LP_solver(n, m, dof_map, dof_count, sol);

		if (success) {
			
			/* Allocate array for scores and values */
			scores =(float*)malloc(BOARDSIZE*2*sizeof(float)); 

			for (i = 0; i < BOARDSIZE; i++) {
				for (j = 0; j < BOARDSIZE; j++) {
					if (B[i][j].num-1 < 0) {						

						/* Find and count all legal values in B[i][j]: */
						k=0; sum_of_weights=0; num_choices=0;
						for (v = 0; v < BOARDSIZE; v++) {
							if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] && is_legal_placement(B, i, j, v+1, n, m) ){
								score = sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ];								
								if (score>=X) {
									num_choices++;						
									sum_of_weights += score*100;								 
									scores[2*k] = score; 	/* store score */
									scores[2*k+1] = v+1; 	/* store value */
									k++;																	
								}
							}							
						}

						/* Choose one of the legal value options for B[i][j]: */
					
						/* Generate a random number from 0 and smaller than sum_of_weights */
						if ( num_choices>0 && ((int) sum_of_weights)>0 ){
							rnd = rand()%((int) sum_of_weights);
						}

						k=0;
						while (k<num_choices) {
							choice = (int) scores[2*k+1];
							if( ( rnd < (int) (scores[2*k]*100) ) && is_legal_placement(B, i, j, choice, n, m) ){							
								B[i][j].num = choice;
								k = num_choices; /* end loop */
							} else {
								rnd -= (int) (scores[2*k]*100);
								k++;
							}
						}
					}
				}		
			}
		}
	}

	/* Free memory solution vector */	
	free(sol);
	free(scores);

	return success;
}

bool LP_guess_hinter(Board B, int n, int m, int i, int j){
	int BOARDSIZE = n*m;
	int	*dof_map = NULL;
	int v, dof_count;
	double	*sol = NULL;
	bool success;
	float score;

	/* Assumption: The board has no errors at this stage. Verified by the calling function. */

	/* Autofill */
	LPILP_autofill(B, n, m);

	printf("Legal values for cell <%d,%d>:\n",j+1,i+1);

	if(has_single_val(B,i,j,&v,n,m)){
		printf("Single value: %d\n",v);
		success = true;
	} else {
	
		/* Count and create a mapping of the variables to be solved for */
		dof_map = (int*)malloc(BOARDSIZE*BOARDSIZE*BOARDSIZE * sizeof(int));
		dof_count = map_variables(B, n, m, dof_map);

		/* Allocate memory for the solution vector: */
		sol = (double*)malloc(dof_count * sizeof(double));
		
		success = LP_solver(n, m, dof_map, dof_count, sol);
		
		if (success) {		
			if (B[i][j].num-1 < 0) {			
				for (v = 0; v < BOARDSIZE; v++) {				
					if (dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v]) {
						score = sol[ (int) dof_map[i*BOARDSIZE*BOARDSIZE+j*BOARDSIZE+v] - 1 ];
						if (score>0) {			
							printf("Value: %d, score:%3.2f\n",v+1,score);											
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