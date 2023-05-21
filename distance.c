#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int n, m;

long D[200][200]; // C-inverse * 16512
#define INV_DIV 16512


void test1()
{
    long f[] = {0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
    show_vector(f, n, "f");


    long prod[115];
    mult_vector(C, f, prod, n);
    show_vector(prod, n, "prod");
    
    long inv[115];
    mult_vector(D, prod, inv, n);
    for(int i=0; i<115; i++) inv[i] /= INV_DIV;
    show_vector(inv, n, "inverted");

}

void test2()
{
    for(int i=0; i<115; i++)
    {
	printf("constraint %d:\n", i);
	double sum = 0;
	for(int j=0; j<115; j++)
	    sum = sum + C[i][j];
	printf("sum: %ld\n", sum);
	for(int j=0; j<115; j++)
	{
	    double tmp = sum - C[i][j];
	    double left = 57 - tmp;
	    left = left / (double) C[i][j];
	    double right = 72 - tmp;
	    right = right / (double) C[i][j];
	}
    }
}

double abs1(double a)
{
    if(a < 0) return -a;
    return a;
}
void round1(double inv[], long rounded[])
{
    for(int i=0; i<115; i++)
	inv[i] = inv[i] / (double)INV_DIV;

    for(int i=0; i<115; i++)
    {
	if(abs1(inv[i] - 0.0) <= abs1(inv[i] - 1.0))
	    rounded[i] = 0;
	else
	    rounded[i] = 1;
    }
}

long error;
void distance()
{
    long f[] = {0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
    long prod[115];
    mult_vector(C, f, prod, n);
    double prod_dist[115];
    double inv[115];
    long rounded[115];

    double error = 3;
    int itr = 10;
    for(int i=0; i<itr; i++)
    {
	for(int j=0; j<115; j++)
	{
	    long mult = (rand() % 3) - 1;
	    prod_dist[j] = ((double)prod[j]) + error * ((double)mult);
	}

	fmult_vector(D, prod_dist, inv, n);
	round1(inv, rounded);

	int x = check1(rounded, C, b, c, n, m);
	int y = same(rounded, f, n);

	printf("itr %d: %d %d", i, x, y);
	if(x == 1 && y == 0) printf("!!!!");
	printf("\n");
    }
}


int main()
{
    srand(time(0));
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    input_matrix(D, "D.txt", &n);
    // show_ilp(C, b, c, n, m);

    test1();
    distance();

    printf("\n\n thikache\n\n");
    return 0;
}
