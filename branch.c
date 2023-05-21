#include <stdio.h>
#include "ilp.h"

long curr[200];

long C[200][200];
long b[200];
long c[200];
int C_n, C_m;
long obj[200];
double sol[200];

// check whether len length prefix in "curr"
// is feasible
int check(int len)
{
    double z, sol[200];
    long obj[200];

    int num_one = 0;
    for(int i=0; i<len; i++)
	num_one += curr[i];
    printf("num_one: %d\n", num_one);

    for(int i=0; i<115; i++) obj[i] = 0;
    for(int i=0; i<len; i++) obj[i] = curr[i];
    z = solve1(C, b, c, obj, sol, 1, 115, 115);
    // printf("----------\n\n");
    if(z < num_one)
	return 0;

    for(int i=0; i<115; i++) obj[i] = 0;
    for(int i=0; i<len; i++) obj[i] = 1 - curr[i];
    z = solve1(C, b, c, obj, sol, 0, 115, 115);
    // printf("----------\n\n");
    if(z > 0)
	return 0;

    return 1;
}

void branch(int bit)
{
    int ch;
    if(bit == 115)
    {
	ch = check1(curr, C, b, c, C_n, C_m);
	if(ch == 1)
	{
	    FILE* fp = fopen("found.txt", "a");
	    for(int i=0; i<115; i++)
		fprintf(fp, "%d", curr[i]);
	    fclose(fp);
	    printf("\n\n\n   FOUND\n\n\n");
	}
	printf("115 bit fail\n");
	return;
    }

    printf("current prefix: ");
    for(int i=0; i<bit; i++)
	printf("%d", curr[i]);
    printf("\n");

    // bit = 0
    curr[bit] = 0;
    ch = check(bit);
    if(ch == 0)
	printf("0 CUT: prefix infeasible\n");
    if(ch == 1)
	branch(bit+1);

    // bit = 1
    curr[bit] = 1;
    ch = check(bit);
    if(ch == 0)
	printf("1 CUT: prefix infeasible\n");
    if(ch == 1)
	branch(bit+1);
}

int main()
{
    input_constraint(C, b, c, "constraint115.txt", &C_n, &C_m);
    // show_ilp(C, b, c, C_n, C_m);

    branch(0);

    printf("\n\n  thikache\n\n");
    return 0;
}
