#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int n, m;

long* arr1;
long* arr2;
long* arr3;
long UPPER;
long LOWER;
int len;
long dp1[115][73][73][73];

// #define INF 2305843009213693951
#define INF 9223372036854775807

void add1(long* a, long b)
{
    long remainder = INF - *a;
    if(b > remainder)
	*a = INF;
    else
	*a = *a + b;
}

// number of ways to keep 4 sub-set sums inside range
long subset_sum_range(int i, int sum1, int sum2, int sum3) 
{
    if(i == len)
    {
	if(sum1 < LOWER || sum1 > UPPER)
	    return 0;
	if(sum2 < LOWER || sum2 > UPPER)
	    return 0;
	if(sum3 < LOWER || sum3 > UPPER)
	    return 0;
	return 1;
    }
    if(dp1[i][sum1][sum2][sum3] != -1)
	return dp1[i][sum1][sum2][sum3];

    long ans = 0;

    if(sum1 + arr1[i] <= UPPER
       && sum2 + arr2[i] <= UPPER
	&& sum3 + arr3[i] <= UPPER)
    {
	add1(&ans, subset_sum_range(i+1, sum1 + arr1[i], sum2 + arr2[i], sum3 + arr3[i]));
    }
    // ans = ans + subset_sum_range(i+1, l, r);
    add1(&ans, subset_sum_range(i+1, sum1, sum2, sum3));
    
	
    dp1[i][sum1][sum2][sum3] = ans;
    return ans;
}

int dp2[115][73][57];
// whether possible to make sum using count size subset
// of arr[i,i+1,...n]
int subset_sum(int i, int sum, int count)
{
    if(i == len)
    {
	if(sum == 0 && count == 0)
	    return 1;
	return 0;
    }

    if(dp2[i][sum][count] != -1)
	return dp2[i][sum][count];

    int check1 = 0, check2 = 0;
    if(arr1[i] <= sum && count >= 1)
	check1 = subset_sum(i+1, sum - arr1[i], count - 1);
    check2 = subset_sum(i+1, sum, count);
    int ans = check1 | check2;

    dp2[i][sum][count] = ans;
    return ans;
}

void reset_dp1()
{
    for(int i=0; i<115; i++)
	for(int j=0; j<73; j++)
	    for(int k=0; k<73; k++)
		for(int l=0; l<73; l++)
		    dp1[i][j][k][l] = -1;
}

void reset_dp2()
{
    for(int i=0; i<115; i++)
	for(int j=0; j<73; j++)
	    for(int k=0; k<57; k++)
		dp2[i][j][k] = -1;
}

void test1()
{
    long tmp_arr1[] = {4, 5, 6, 2};
    long tmp_arr2[] = {4, 5, 6, 3};
    long tmp_arr3[] = {4, 5, 7, 3};

    arr1 = tmp_arr1;
    arr2 = tmp_arr2;
    arr3 = tmp_arr3;
    len = 4;
    LOWER = 10;
    UPPER = 11;
    reset_dp1();

    long ans = subset_sum_range(0, 0, 0, 0);
    printf("test ans = %ld\n", ans);

    printf("\n\n");
}

void range()
{
    len = n;
    LOWER = 57;
    UPPER = 72;

    int rand1, rand2, rand3;
    for(int i=0; i<n; i++)
    {
	// rand1 = 0; // rand() % n;
	arr1 = C[i];
	for(int j=i+1; j<n; j++)
	{
	    // rand2 = 93; // rand() % n;
	    arr2 = C[j];
	    for(int k=j+1; k<n; k++)
	    {
		// rand3 = 114;
		arr3 = C[k];

		reset_dp1();
		long ans = subset_sum_range(0, 0, 0, 0);
		printf("%d %d %d: %ld\n", i, j, k, ans);
		
	    }
	}
    }
    
}

void test2()
{
    len = n;
    arr1 = C[114];
    for(int i=57; i<=72; i++)
    {
	reset_dp2();
	int ans = subset_sum(0, i, 57);
	printf("%d: %d\n", i, ans);
    }
    printf("\n\n");
}

int main()
{
    srand(time(0));
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    // show_ilp(C, b, c, n, m);

    // test1();
    // return 0;

    range();


    printf("\n\n  thikache\n\n");
    return 0;
}

