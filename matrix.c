#include <stdio.h>
#include <stdlib.h>

double** create_matrix(int n, int m)
{
    double** mat = (double**)malloc(n*sizeof(double*));
    for(int i=0; i<n; i++)
	mat[i] = (double*)malloc(m * sizeof(double));
    return mat;
}

void destroy_matrix(double** A, int num_rows)
{
    for(int i=0; i<num_rows; i++)
	free(A[i]);
    free(A);
}

// copy column b[y] into a[x]
void copy_column(double **a, int x, double** b, int y, int num_rows)
{
    for(int i=0; i<num_rows; i++)
	a[i][x] = b[i][y];
}

// swap rows a[x] and a[y]
double swap_row(double** a, int n, int x, int y)
{
    for(int j=0; j<n; j++)
    {
	double tmp  = a[x][j];
	a[x][j] = a[y][j];
	a[y][j] = tmp;
    }
}

// store a inverse in b
void inverse(double** a, int n, double** b)
{
    /* for(int j=0; j<n; j++) */
    /* { */
    /* 	if(a[j][j] == 0) */
    /* 	    pivot(a, n, ) */
    /* } */
}

