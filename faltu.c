#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int C_n;
int C_m;


void get_rand(long vec[])
{
    long list[112]; for(int i=0; i<112; i++) list[i] = i;
    list[0] = 112;
    list[93] = 113;

    for(int i=0; i<115; i++) vec[i] = 0;
    vec[93] = 1;

    int len = 112;
    for(int i=0; i<56; i++)
    {
	int idx = rand() % len;
	vec[list[idx]] = 1;
	list[idx] = list[len-1];
	len--;
    }
}

int weight(long vec[])
{
    int ans = 0;
    for(int i=0; i<115; i++) ans = ans + vec[i];
    return ans;
}

void parity()
{
    for(int i=0; i<115; i++)
    {
	for(int j=0; j<115; j++)
	{
	    if(C[i][j] & 1)
		printf("1");
	    else
		printf(" ");
	}
	printf("\n");
    }
    printf("\n\n");
}

void test1()
{
    long vec[115]; long prod[115];
    int itr = 100;
    while(itr--)
    {
	get_rand(vec);
	printf("weight(vec): %d\n", weight(vec));

	mult_vector(C, vec, prod, 115);

	// show_vector(prod, 115, "Ax");
	for(int i=0; i<115; i++)
	{
	    if(prod[i] < 57 || prod[i] > 72)
		printf("%d: %ld\n", i, prod[i]);
	}
	printf("\n\n");
    }
}

int main()
{
    srand(time(0));

    input_constraint(C, b, c, "constraint115.txt", &C_n, &C_m);
    // show_ilp(C, b, c, C_n, C_m);

    parity();

    return 0;
}
