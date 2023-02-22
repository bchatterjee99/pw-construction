#include <stdio.h>
#include <glpk.h>


// GNU linear programming kit diye
void solve(long Constraint[][200], long b[], long c[], int n, int m)
{
    glp_prob *lp;
    int ia[1+40000], ja[1+40000];
    double ar[1+40000], z, x1, x2, x3;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "thikache");

    glp_set_obj_dir(lp, GLP_MAX);

    glp_add_rows(lp, n);
    for(int i=0; i<n; i++)
    {
	if(b[i] == c[i])
	    glp_set_row_bnds(lp, i+1, GLP_FX, b[i], c[i]);
	else
	    glp_set_row_bnds(lp, i+1, GLP_DB, b[i], c[i]);
    }

    glp_add_cols(lp, m);
    for(int j=0; j<m; j++)
    {
	// glp_set_col_bnds(lp, j, GLP_LO, 0.0, 0.0);
	glp_set_col_kind(lp, j+1, GLP_BV);
	glp_set_obj_coef(lp, j+1, 1);
    }

    for(int i=0; i<n; i++)
    {
	for(int j=0; j<m; j++)
	{
	    int idx = i * m  + j + 1;
	    // printf("idx = %d,  r: %d, c: %d\n", idx, i+1, j+1);
	    ia[idx] = i + 1;
	    ja[idx] = j + 1;
	    ar[idx] = Constraint[i][j];
	}
	
    }

    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.presolve = GLP_ON;

    glp_load_matrix(lp, n*m, ia, ja, ar);
    glp_intopt(lp, &parm);
    z = glp_mip_obj_val(lp);
    printf("obj val: %lf\n", z);
    for(int j=0; j<m; j++)
    {
	double x = glp_mip_col_val(lp, j+1);
	printf("x%d: %lf, ", j+1, x);
    }
    printf("\n\n");
    glp_delete_prob(lp);
}



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

// returns distance from failed constraints
int check(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m)
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

void check1(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m)
{
    printf("Check:\n");
    int flag = 0;
    for(int i=0; i<C_n; i++)
    {
	long tmp = 0;
	for(int j=0; j<C_m; j++)
	    tmp = tmp + f[j] * Constraint[i][j];
	if(tmp < b[i] || tmp > c[i])
	{
	    printf("constraint %d failed\n", i);
	    printf("b = %ld, tmp = %ld, c = %ld\n\n", b[i], tmp, c[i]);
	    flag = 1;
	}
    }
    if(flag == 0) printf("passed\n");
    else printf("failed\n");
    printf("\n\n");
}
