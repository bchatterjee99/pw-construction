#include <stdio.h>
#include <stdlib.h>
#include "ilp.h"
#include <glpk.h>

#define TOTAL 13225
#define ROW 115
#define COL 115

long C[200][200];
long b[200];
long c[200];
int C_n, C_m;
long obj[200];
double sol[200];

void test2()
{
    long C[200][200], b[2], c[2];
    long obj[2]; double sol[2];

    b[0] = 0; C[0][0] = 3; C[0][1] = 4; c[0] = 12;
    b[1] = 0; C[1][0] = 1; C[1][1] = 0; c[1] = 2;
    obj[0] = 1; obj[1] = 1;

    double z = solve1(&C[0], b, c, obj, sol, 1, 2, 2);
    printf("----------\n\n");

    printf("SIMPLEX returned:\n");
    printf("z = %lf\n", z);
    printf("sol: %lf, %lf\n", sol[0], sol[1]);
    printf("----------\n\n");
}

void test3()
{
    input_constraint(C, b, c, "constraint115.txt", &C_n, &C_m);
    // show_ilp(C, b, c, C_n, C_m);
    long obj[200]; double sol[200];

    for(int i=0; i<115; i++)
	obj[i] = 0;

    obj[0] = 1;
    for(int i=1; i<40; i++)
    {
	obj[i] = 1;
	double z = solve1(C, b, c, obj, sol, 1, 115, 115);
	printf("----------\n\n");
	printf("SIPMLEX returned:\n");
	printf("i = %d -- ", i);
	printf("z = %lf\n", z);
	/* printf("sol:\n"); */
	/* for(int j=0; j<115; j++) */
	/* 	printf("x[%d]: %lf\n", j+1, sol[j]); */
	printf("\n\n");
    }
}

void int_sol1(double sol[])
{
    long int_sol[200];
    printf("int-sol:\n");
    for(int i=0; i<115; i++)
    {
	if(i & 1)
	{
	    if(sol[i] != 0.0)
		sol[i] = 1;
	}
	else
	{
	    if(sol[i] != 1.0)
		sol[i] = 0;
	}
	int_sol[i] = sol[i];
	printf("x[%d]: %d\n", i+1, int_sol[i]);
    }
    int ch = check1(int_sol, C, b, c, C_n, C_m);
    printf("ch = %d\n", ch);
    printf("\n\n");
}

void test4()
{
    input_constraint(C, b, c, "constraint115.txt", &C_n, &C_m);
    // show_ilp(C, b, c, C_n, C_m);
    long obj[200]; double sol[200];

    for(int i=0; i<115; i++)
	// obj[i] = 1 * ((-1) * (i & 1));
	obj[i] = 1;

    double z = solve1(C, b, c, obj, sol, 1, 115, 115);
    printf("----------\n\n");
    printf("SIPMLEX returned:\n");
    printf("z = %lf\n", z);
    printf("sol:\n");
    for(int j=0; j<115; j++)
	printf("x[%d]: %lf\n", j+1, sol[j]);
    printf("\n\n");

}

int main()
{
    test4();

    printf("\n\n  thikache\n\n");
    return 0;
}
