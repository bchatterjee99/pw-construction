#include <stdio.h>
#include <stdlib.h>

#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int n;
int m;


void show_ilp()
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

long selection[28];
long counter;
void test_selection()
{
    long x[112]; int idx = 0;
    for(int i=0; i<28; i++) // expand selection to 112 bits
    {
	x[idx++] = selection[i];
	x[idx++] = selection[i];
	x[idx++] = selection[i];
	x[idx++] = selection[i];
    }

    // construct ilp vector
    long y[115]; idx = 0;
    for(int i=0; i<115; i++)
    {
	if(i == 0) y[i] = 0;
	else if(i == 93) y[i] = 1;
	else if(i == 114) y[i] = 0;
	else y[i] = x[idx++];
    }

    /* printf("y = "); int y_count = 0; */
    /* for(int i=0; i<115; i++) */
    /* { */
    /* 	y_count = y_count + y[i]; */
    /* 	// printf("%d", y[i]); */
    /* } */
    /* printf("y_count = %d\n", y_count); */

    int z = check1(y, C, b, c, n, m);
    if(z == 1)
    {
	printf("pass: ");
	for(int i=0; i<115; i++)
	    printf("%ld", y[i]);
	printf("\n");
	exit(1);
    }
}

void select1(int pos, int num)
{
    if(pos == 28)
    {
	/* int count = 0; */
	/* for(int i=0; i<28; i++) */
	/* { */
	/*     printf("%d", selection[i]); */
	/*     count = count + selection[i]; */
	/* } */
	/* printf("count = %d", count); */
	/* printf("\n"); */
	test_selection();
	counter++;
	printf("tested: %ld / 40116600\n", counter);
	return;
    }
    if(28 - pos > num)
    {
	selection[pos] = 0;
	select1(pos + 1, num);
    }
    if(num > 0)
    {
	selection[pos] = 1;
	select1(pos + 1, num - 1);
    }
}

int main()
{
    input_constraint(C, b, c, "constraint115.txt", &n, &m);

    counter = 0;
    select1(0, 14);
    printf("counter = %ld\n", counter);

    printf("\n\n  thikache\n\n");
    return 0;
}
