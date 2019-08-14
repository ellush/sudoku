/*
  Sudoku solver.

  The Sudoku board is a 9x9 grid, which is further divided into a 3x3 grid
  of 3x3 grids.  Each cell in the grid must take a value from 0 to 9.
  No two grid cells in the same row, column, or 3x3 subgrid may take the
  same value.

  In the MIP formulation, binary variables x[i,j,v] indicate whether
  cell <i,j> takes value 'v'.  The constraints are as follows:
    1. Each cell must take exactly one value (sum_v x[i,j,v] = 1)
    2. Each value is used exactly once per row (sum_i x[i,j,v] = 1)
    3. Each value is used exactly once per column (sum_j x[i,j,v] = 1)
    4. Each value is used exactly once per 3x3 subgrid (sum_grid x[i,j,v] = 1)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"

#define SUBDIM  3
#define DIM    (SUBDIM*SUBDIM)

int
main(int   argc,
     char *argv[])
{
  FILE     *fp    = NULL;
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       board[DIM][DIM];
  char      inputline[100];
  int       ind[DIM];
  double    val[DIM];
  char      vtype[DIM*DIM*DIM];
  char     *names[DIM*DIM*DIM];
  char      namestorage[10*DIM*DIM*DIM];
  char     *cursor;
  int       optimstatus;
  double    objval;
  int       i, j, k, v, ig, jg, count;
  int       error = 0;
  double    sol[DIM*DIM*DIM];
  
  double    dof_map[DIM*DIM*DIM];
  int       dof_count = 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: sudoku_c datafile\n");
    exit(1);
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: unable to open input file %s\n", argv[1]);
    exit(1);
  }

  for (i = 0; i < DIM; i++) {
    fgets(inputline, 100, fp);
    if (strlen(inputline) < 9) {
      fprintf(stderr, "Error: not enough board positions specified\n");
      exit(1);
    }
    for (j = 0; j < DIM; j++) {
      board[i][j] = (int) inputline[j] - (int) '1';
      if (board[i][j] < 0 || board[i][j] >= DIM)
        board[i][j] = -1;
    }
  }

  /* Initialize condensed array for differentiating between redundant and required variables. */
  for (i = 0; i < DIM*DIM*DIM; i++) {
    dof_map[i]=-1;
  }

  /*  Create an empty model, 
      While removing redundant variables 
  */
  cursor = namestorage;
  for (i = 0; i < DIM; i++) {
    for (j = 0; j < DIM; j++) {
      if (board[i][j] >= 0) {
        for (v = 0; v < DIM; v++){
          dof_map[i*DIM*DIM+j*DIM+v] = 0;
          if (board[i][j]==v) {                        
            for (k = 0; k < DIM; k++) {
              dof_map[k*DIM*DIM+j*DIM+v] = 0;
              dof_map[i*DIM*DIM+k*DIM+v] = 0;
            }
            for (ig = (i/SUBDIM)*SUBDIM; ig < (i/SUBDIM+1)*SUBDIM; ig++) {              
              for (jg = (j/SUBDIM)*SUBDIM; jg < (j/SUBDIM+1)*SUBDIM; jg++) { 
                dof_map[ig*DIM*DIM+jg*DIM+v] = 0;
              }
            }
          }
        }         
      }
    }
  }

  /* Index the variables solved for */
  for (i = 0; i < DIM; i++) {
    for (j = 0; j < DIM; j++) {
      for (v = 0; v < DIM; v++) {
        if (dof_map[i*DIM*DIM+j*DIM+v]<0) {
          vtype[dof_count] = GRB_BINARY;
          names[dof_count] = cursor;
          sprintf(names[dof_count], "x[%d,%d,%d]", i, j, v+1);
          cursor += strlen(names[dof_count]) + 1;
          dof_count++;
          dof_map[i*DIM*DIM+j*DIM+v] = dof_count;                   
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
  for (i = 0; i < DIM; i++) {
    for (j = 0; j < DIM; j++) {
      count = 0;
      for (v = 0; v < DIM; v++) {
        if (dof_map[i*DIM*DIM + j*DIM + v]) {
          ind[count] = dof_map[i*DIM*DIM + j*DIM + v]-1;
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

  for (v = 0; v < DIM; v++) {
    for (j = 0; j < DIM; j++) {
      count = 0;
      for (i = 0; i < DIM; i++) {
       if (dof_map[i*DIM*DIM + j*DIM + v]) {
          ind[count] = dof_map[i*DIM*DIM + j*DIM + v]-1;
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

 for (v = 0; v < DIM; v++) {
    for (i = 0; i < DIM; i++) {
      count = 0;
      for (j = 0; j < DIM; j++) {
        if (dof_map[i*DIM*DIM + j*DIM + v]) {
          ind[count] = dof_map[i*DIM*DIM + j*DIM + v]-1;
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

  for (v = 0; v < DIM; v++) {        
    for (ig = 0; ig < SUBDIM; ig++) {
      for (jg = 0; jg < SUBDIM; jg++) {        
        count = 0;
        for (i = ig*SUBDIM; i < (ig+1)*SUBDIM; i++) {
          for (j = jg*SUBDIM; j < (jg+1)*SUBDIM; j++) {
            if (dof_map[i*DIM*DIM + j*DIM + v]) {
              ind[count] = dof_map[i*DIM*DIM + j*DIM + v]-1;
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

    printf("\nOriginal board:\n");
    for (i = 0; i < DIM; i++) {
      for (j = 0; j < DIM; j++) {
        if (board[i][j] < 0)
          printf("x ");
        else
          printf("%d ", board[i][j]+1 );
      }
      printf("\n");
    }

    printf("\nSolution:\n");
    for (i = 0; i < DIM; i++) {
      for (j = 0; j < DIM; j++) {
        if (board[i][j] < 0)
          for (v = 0; v < DIM; v++) {
          if (dof_map[i*DIM*DIM+j*DIM+v])          
            if (sol[ (int) dof_map[i*DIM*DIM+j*DIM+v] - 1 ]>0) {
              printf("%d ", v+1);
            }
        }
        else
          printf("%d ", board[i][j]+1 );
      }
      printf("\n");
    }
    printf("\n");
        
  } else if (optimstatus == GRB_INF_OR_UNBD)
    printf("Model is infeasible or unbounded\n");
  else
    printf("Optimization was stopped early\n");
  printf("\n");

  QUIT:

  /* Error reporting */

  if (error) {
    printf("ERROR: %s\n", GRBgeterrormsg(env));
    exit(1);
  }

  /* Free model */

  GRBfreemodel(model);

  /* Free environment */

  GRBfreeenv(env);

  return 0;
}
