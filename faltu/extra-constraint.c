#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int n, m;

long surrogate[200];
long sb, sc;


void test1()
{
    for(int i=0; i<115; i++) surrogate[i] = 0;

    for(int i=0; i<55; i++)
	for(int j=0; j<115; j++)
	    surrogate[j] = surrogate[j] + C[i][j];
    sb = 57 * 115;
    sc = 72 * 115;

    for(int i=0; i<115; i++)
	printf("surr[%d] = %ld\n", i, surrogate[i]);
    printf("\n\n");
}

int main()
{
    srand(time(0));
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    // show_ilp(C, b, c, n, m);

    test1();

    printf("\n\n  thikache\n\n");
    return 0;
}
