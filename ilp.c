#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#include "matrix.h"

/* // GNU linear programming kit diye */
/* void solve(long Constraint[][200], long b[], long c[], int n, int m) */
/* { */
/*     glp_prob *lp; */
/*     int ia[1+40000], ja[1+40000]; */
/*     double ar[1+40000], z, x1, x2, x3; */
/*     lp = glp_create_prob(); */
/*     glp_set_prob_name(lp, "thikache"); */

/*     glp_set_obj_dir(lp, GLP_MAX); */

/*     glp_add_rows(lp, n); */
/*     for(int i=0; i<n; i++) */
/*     { */
/* 	if(b[i] == c[i]) */
/* 	    glp_set_row_bnds(lp, i+1, GLP_FX, b[i], c[i]); */
/* 	else */
/* 	    glp_set_row_bnds(lp, i+1, GLP_DB, b[i], c[i]); */
/*     } */

/*     glp_add_cols(lp, m); */
/*     for(int j=0; j<m; j++) */
/*     { */
/* 	// glp_set_col_bnds(lp, j, GLP_LO, 0.0, 0.0); */
/* 	glp_set_col_kind(lp, j+1, GLP_BV); */
/* 	glp_set_obj_coef(lp, j+1, 1); */
/*     } */

/*     for(int i=0; i<n; i++) */
/*     { */
/* 	for(int j=0; j<m; j++) */
/* 	{ */
/* 	    int idx = i * m  + j + 1; */
/* 	    // printf("idx = %d,  r: %d, c: %d\n", idx, i+1, j+1); */
/* 	    ia[idx] = i + 1; */
/* 	    ja[idx] = j + 1; */
/* 	    ar[idx] = Constraint[i][j]; */
/* 	} */
	
/*     } */

/*     glp_iocp parm; */
/*     glp_init_iocp(&parm); */
/*     parm.presolve = GLP_ON; */

/*     glp_load_matrix(lp, n*m, ia, ja, ar); */
/*     glp_intopt(lp, &parm); */
/*     z = glp_mip_obj_val(lp); */
/*     printf("obj val: %lf\n", z); */
/*     for(int j=0; j<m; j++) */
/*     { */
/* 	double x = glp_mip_col_val(lp, j+1); */
/* 	printf("x%d: %lf, ", j+1, x); */
/*     } */
/*     printf("\n\n"); */
/*     glp_delete_prob(lp); */
/* } */



void input_matrix(long A[][200], const char* file, int* n)
{
    FILE* fp = fopen(file, "r");
    fscanf(fp, "%d", n);
    int max_digits = 0;
    long tmp;

    // A
    for(int i=0; i<*n; i++)
    {
	for(int j=0; j<*n; j++)
	{
	    fscanf(fp, "%ld", &tmp);
	    A[i][j] = tmp;
	}
    }
    fclose(fp);
}


void input_constraint(long A[][200], long b[], long c[], const char* file, int* C_n, int* C_m)
{
    FILE* fp = fopen(file, "r");

    fscanf(fp, "%d", C_n);
    fscanf(fp, "%d", C_m);

    for(int i=0; i<*C_n; i++)
    {
	for(int j=0; j<*C_m; j++)
	    fscanf(fp, "%d", &A[i][j]);
    }

    for(int i=0; i<*C_n; i++)
	fscanf(fp, "%d", &b[i]);


    for(int i=0; i<*C_n; i++)
	fscanf(fp, "%d", &c[i]);
}


void show_matrix(long A[][200], int n, char* str)
{
    printf("%s:\n", str);
    for(int i=0; i<n; i++)
    {
	for(int j=0; j<n; j++)
	    printf("%d ", A[i][j]);
	printf("\n");
    }
    printf("\n\n");
}
void show_vector(long b[], int n, char* str)
{
    printf("%s:\n", str);
    for(int i=0; i<n; i++)
    {
	printf("%d ", b[i]);
    }
    printf("\n\n");
}
void show_ilp(long C[][200], long b[], long c[], int n, int m)
{
    for(int i=0; i<n; i++)
    {
	for(int j=0; j<m; j++)
	    printf("%ld ", C[i][j]);
	printf("\n");
    }
    
    printf("n = %d m = %d\n", n, m);
    printf("b = \n");
    for(int i=0; i<m; i++) printf("%ld ", b[i]); printf("\n");
    printf("c = \n");
    for(int i=0; i<m; i++) printf("%ld ", c[i]); printf("\n");

    printf("\n\n");
}

void mult_matrix(long A[][200], long B[][200], long C[][200], int n)
{
    for(int i=0; i<n; i++)
    {
	for(int j=0; j<n; j++)
	{
	    C[i][j] = 0;
	    for(int k=0; k<n; k++)
		C[i][j] = C[i][j] + A[i][k] * B[k][j];
	}
    }
}
void mult_vector(long A[][200], long b[], long c[], int n)
{
    for(int i=0; i<n; i++)
    {
	    c[i] = 0;
	    for(int k=0; k<n; k++)
		c[i] = c[i] + A[i][k] * b[k];
    }
}
void fmult_vector(long A[][200], double b[], double c[], int n)
{
    for(int i=0; i<n; i++)
    {
	    c[i] = 0;
	    for(int k=0; k<n; k++)
		c[i] = c[i] + ((double) A[i][k]) * b[k];
    }
}

// returns distance from failed constraints
int check2(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m)
{
    // printf("Check:\n");
    int dist = 0;
    for(int i=0; i<C_n; i++)
    {
	long tmp = 0;
	for(int j=0; j<C_m; j++)
	    tmp = tmp + f[j] * Constraint[i][j];
	if(tmp < b[i] || tmp > c[i])
	{
	    //printf("constraint %d failed\n", i);
	    //printf("b = %ld, tmp = %ld, c = %ld\n\n", b[i], tmp, c[i]);
	    if(tmp < b[i])
		dist = dist + b[i] - tmp;
	    else
		dist = dist + tmp - c[i];
	}
    }
    /* if(flag == 0) printf("passed\n"); */
    /* else printf("failed\n"); */
    /* printf("\n\n"); */
    return dist;
}

int check1(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m)
{
    int flag = 0;
    for(int i=0; i<C_n; i++)
    {
	long tmp = 0;
	for(int j=0; j<C_m; j++)
	    tmp = tmp + f[j] * Constraint[i][j];
	if(tmp < b[i] || tmp > c[i])
	{
	    /* printf("constraint %d failed\n", i); */
	    /* printf("b = %ld, tmp = %ld, c = %ld\n\n", b[i], tmp, c[i]); */
	    flag = 1;
	    return 0; // return on first fail
	}
    }
    if(flag == 0) return 1;
    return 0;
}

int same(long a[], long b[], int n)
{
    for(int i=0; i<n; i++)
	if(a[i] != b[i])
	    return 0;
    return 1;
}



double solve1(long C[][200], long b[], long c[], long obj[], double sol[], int dir, int n, int m)
{
    int num_row = 2 * n;
    int num_col = m + 1;

    // intialize
    glp_prob *lp;
    glp_smcp parm;
    int *ia, *ja; double *arr;
    ia = (int*)malloc((num_row * num_col + 1) * sizeof(int));
    ja = (int*)malloc((num_row * num_col + 1) * sizeof(int));
    arr = (double*)malloc((num_row * num_col + 1) * sizeof(double));
    double z;

    // setup
    lp = glp_create_prob();
    glp_set_prob_name(lp, "aaa");
    if(dir == 0) // minimization
	glp_set_obj_dir(lp, GLP_MIN);
    else // maximization
	glp_set_obj_dir(lp, GLP_MAX);
    glp_add_rows(lp, num_row);
    glp_add_cols(lp, num_col);
    glp_init_smcp(&parm);
    parm.msg_lev = GLP_MSG_OFF;


    // constraints
    int k = 1;
    for(int i=0; i<n; i++)
    {
	// upper
	ia[k] = 2*i + 1;
	ja[k] = 1;
	arr[k] = c[i];
	k++;
	for(int j=0; j<m; j++)
	{
	    ia[k] = 2*i+1;
	    ja[k] = j+2;
	    arr[k] =  - C[i][j];
	    k++;
	}
	glp_set_row_bnds(lp, 2*i+1, GLP_LO, 0.0, 0.0);

	// lower
	ia[k] = 2*i + 2;
	ja[k] = 1;
	arr[k] = - b[i];
	k++;
	for(int j=0; j<m; j++)
	{
	    ia[k] = 2*i+2;
	    ja[k] = j+2;
	    arr[k] = C[i][j];
	    k++;
	}
	glp_set_row_bnds(lp, 2*i+2, GLP_LO, 0.0, 0.0);
    }

    // objective
    for(int i=0; i<m; i++)
	glp_set_obj_coef(lp, i+1, obj[i]);

    // column bounds
    for(int i=0; i<m; i++) // keep 0 - 1
	// glp_set_col_bnds(lp, i+1, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp, i+1, GLP_DB, 0.0, 1.0);

    // solve
    glp_load_matrix(lp, (2*n)*m, ia, ja, arr);
    glp_simplex(lp, &parm);

    // solution
    z = glp_get_obj_val(lp);
    // printf("solution:\n");
    for(int i=0; i<m; i++)
    {
	double tmp = glp_get_col_prim(lp, i+1);
	// printf("x[%d]: %lf\n", i+1, tmp);
	sol[i] = tmp;
    }

    glp_print_sol(lp, "simplex.txt");
    // find_basic(C, b, c, sol, n, m, lp);

    glp_delete_prob(lp);
    return z;
}
