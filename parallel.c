#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ilp.h"

long C[200][200];
long b[200];
long c[200];
int n;
int m;

// generate LEN length strings
// with NUM_ONE 1's
#define LEN 28
#define NUM_ONE 14

#define CORES 5

long ncr[60][60];
long combinations(int n, int r)
{
    if(r > n) return 0;
    if(r == n) return 1;
    if(n == 0) return 1; // r = 0
    if(n == 1) return 1; // r = 0 or 1
    if(r == 0) return 1;
    if(r == 1) return n;

    if(ncr[n][r] != -1) return ncr[n][r];
    long ans = combinations(n-1, r-1) + combinations(n-1, r);
    ncr[n][r] = ans;

    return ans;
}


int arr1[200];
// find kth string of length len
// with num_one 1's
void generate(int len, int num_one, long k)
{
    if(num_one == 0) return;
    if(len == 0)
    {
	printf("generate() error: len = 0, num_one != 0\n\n");
	exit(1);
    }
    // find position of first 1
    long done = 0;
    for(int i=num_one-1; i<len; i++)
    {
	// number of strings
	// length i
	// 1 less num 1's
	long num_strings = combinations(i, num_one - 1);
	if(done + num_strings >= k) 
	{
	    arr1[i] = 1;
	    generate(i, num_one - 1, k - done);
	    break;
	}
	done = done + num_strings;
    }
}

// find next string of length LEN
// with NUM_ONE 1's
int arr2[200];
void next()
{
    // find first movable 1
    int count = 0;
    for(int i=0; i<LEN-1; i++)
    {
	if(arr2[i] == 1 && arr2[i+1] == 0)
	{
	    arr2[i+1] = 1;

	    // put count 1s at the right
	    for(int j=0; j<=i; j++)
		arr2[j] = 0;
	    for(int j=0; j<count; j++)
		arr2[j] = 1;

	    break;
	}
	else if(arr2[i] == 1)
	    count++; // #1's seen
    }
    // no change if last possible
}

void show_string(int arr[])
{
    for(int i=LEN-1; i>=0; i--)
	printf("%d", arr[i]);
}

void init()
{
    for(int i=0; i<60; i++)
	for(int j=0; j<60; j++)
	    ncr[i][j] = -1;

    for(int i=0; i<LEN; i++) arr1[i] = 0;
    for(int i=0; i<LEN; i++) arr2[i] = 0;
}

void combinations_test()
{
    long n, r, ans;

    n = 5; r = 0;
    ans = combinations(n, r);
    printf("%ld c %ld :  %ld", n, r, ans);
}

void generate_test()
{
    generate(LEN, NUM_ONE, 1000);
    printf("generated string:\n");
    show_string(arr1);
    printf("\n\n");
    
}

void next_test()
{
    for(int i=0; i<NUM_ONE; i++)
	arr2[i] = 1;

    // show first few strings
    for(int i=0; i<1050; i++)
    {
	if(i+1 == 1000)
	{
	    printf("string %d: ", i+1);
	    show_string(arr2);
	    printf("\n");
	}
	next();
    }
    printf("\n\n");
}

// test arr2
void test_string()
{
    int arr3[112];
    int comb = 112 / LEN; // koto gulo jora hobe
    int idx = 0;
    for(int i=0; i<LEN; i++)
	for(int j=0; j<comb; j++)
	    arr3[idx++] = arr2[i];

    long arr4[115]; idx = 0;
    for(int i=0; i<115; i++)
    {
	if(i == 0) arr4[i] = 0;
	else if(i == 93) arr4[i] = 1;
	else if(i == 114) arr4[i] = 0;
	else
	    arr4[i] = arr3[idx++];
    }

    // test string
    /* int sum = 0; */
    /* for(int i=0; i<115; i++) */
    /* { */
    /* 	printf("%d ", arr4[i]); */
    /* 	sum = sum + arr4[i]; */
    /* } */
    /* printf("sum = %d\n\n", sum); */

    int x = check1(arr4, C, b, c, n, m);
    if(x == 1)
    {
	printf("pass:\n");
	for(int i=0; i<115; i++)
	    printf("%d ", arr4[i]);
	printf("\n\n");
	exit(1);
    }
}

// test num strings starting
// from the kth string
void search(int k, int num)
{
    generate(LEN, NUM_ONE, k);
    for(int i=0; i<LEN; i++) arr2[i] = arr1[i];

    // printf("serach:\n");
    for(int i=0; i<num; i++)
    {
	// show_string(arr2);
	// printf("\n");
	test_string();
	next();
    }
}

void start()
{
    int total = combinations(LEN, NUM_ONE);
    int per_process = total / CORES + 1;
    int done = 0;

    int i;
    for(i=0; i<CORES; i++)
    {
	int pid = fork();
	if(pid != 0)
	    break;
    }
    if(i == CORES) exit(1);
    printf("process %d: i = %d\n", getpid(), i);

    search(i * per_process + 1, per_process);

    int status;
    while(wait(&status) > 0); // wait for child process
    printf("process %d end\n", getpid());
}

int main()
{
    init();
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    // show_ilp(C, b, c, n ,m);

    /* next_test(); */
    /* generate_test(); */
    start();

    printf("\n\n  thikache\n\n");
    return 0;
}
