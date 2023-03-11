#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ilp.h"

#define INF 2305843009213693951

long C[200][200];
long b[200];
long c[200];
int n, m;

long dp[73][73][73][73];
long old_dp[73][73][73][73];
long* arr1;
long* arr2;
long* arr3;
long* arr4;

void add1(long* a, long b)
{
    long remainder = INF - *a;
    if(b > remainder)
	*a = INF;
    else
	*a = *a + b;
}


void copy_table()
{
    for(int i=0; i<73; i++)
	for(int j=0; j<73; j++)
	    for(int k=0; k<73; k++)
		for(int l=0; l<73; l++)
		    old_dp[i][j][k][l] = dp[i][j][k][l];
}

void save_table(int i, int j, int k, int l)
{
    FILE* fp = fopen("./faltu/table1.txt", "w");

    fprintf(fp, "%ld %ld %ld %ld\n", i, j, k, l);

    for(int i=0; i<73; i++)
	for(int j=0; j<73; j++)
	    for(int k=0; k<73; k++)
		for(int l=0; l<73; l++)
		    fprintf(fp, "%ld ", dp[i][j][k][l]);

    fclose(fp);
}

void solve()
{
    for(int i=0; i<73; i++)
	for(int j=0; j<73; j++)
	    for(int k=0; k<73; k++)
		for(int l=0; l<73; l++)
		    old_dp[i][j][k][l] = 0;
    old_dp[0][0][0][0] = 1;

    for(int idx=0; idx<115; idx++)
    {
	printf("idx = %d\n", idx);
	for(int i=0; i<73; i++)
	{
	    for(int j=0; j<73; j++)
	    {
		for(int k=0; k<73; k++)
		{
		    for(int l=0; l<73; l++)
		    {
			dp[i][j][k][l] = 0;
			int i1 = i - arr1[idx];
			int j1 = j - arr2[idx];
			int k1 = k - arr3[idx];
			int l1 = l - arr4[idx];
			if(i1 >= 0 &&
			   j1 >= 0 &&
			   k1 >= 0 &&
			   l1 >= 0)
			    add1(&dp[i][j][k][l], old_dp[i1][j1][k1][l1]);
			// dp[i][j][k][l] += old_dp[i1][j1][k1][l1];

			// dp[i][j][k][l] += old_dp[i][j][k][l];
			add1(&dp[i][j][k][l], old_dp[i][j][k][l]);
		    }
		}
	    }
	}
	copy_table();
    }
    
}

void check_range()
{
    printf("check_range():\n");
    for(int i=57; i<73; i++)
    {
	for(int j=57; j<73; j++)
	{
	    for(int k=57; k<73; k++)
	    {
		for(int l=57; l<73; l++)
		{
		    if(dp[i][j][k][l] == 0 || dp[i][j][k][l] == INF) continue; 
		    printf("%d %d %d %d: ", i, j, k, l);
		    printf("%ld\n", dp[i][j][k][l]);
		}
	    }
	}
    }
}

void test()
{
    int itr = 1, stop = 0;
    for(int i=0; i<115; i++)
    {
	arr1 = C[i];
	for(int j=i+1; j<115; j++)
	{
	    arr2 = C[j];
	    for(int k=j+1; k<115; k++)
	    {
		arr3 = C[k];
		for(int l=k+1; l<115; l++)
		{
		    arr4 = C[l];
		    solve();
		    save_table(i, j, k, l);
		    check_range();

		    // stop
		    itr--;
		    if(itr == 0) { stop = 1; break; }
		}
		if(stop) break;
	    }
	    if(stop) break;
	}
	if(stop) break;
    }

}
int main()
{
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    show_ilp(C, b, c, n, m);

    test();

    printf("\n\n  thikache\n\n");
    return 0;
}
