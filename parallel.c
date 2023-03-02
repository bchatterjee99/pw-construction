#include <omp.h>
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

#define CORES 4
int real_num_threads;
long total;

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


// find kth string of length len
// with num_one 1's
void generate(int len, int num_one, long k, int arr1[])
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
	    generate(i, num_one - 1, k - done, arr1);
	    break;
	}
	done = done + num_strings;
    }
}

// find next string of length LEN
// with NUM_ONE 1's
void next(int arr1[])
{
    // find first movable 1
    int count = 0;
    for(int i=0; i<LEN-1; i++)
    {
	if(arr1[i] == 1 && arr1[i+1] == 0)
	{
	    arr1[i+1] = 1;

	    // put count 1s at the right
	    for(int j=0; j<=i; j++)
		arr1[j] = 0;
	    for(int j=0; j<count; j++)
		arr1[j] = 1;

	    break;
	}
	else if(arr1[i] == 1)
	    count++; // #1's seen
    }
    // no change if last possible
}

void show_string(int arr[])
{
    for(int i=LEN-1; i>=0; i--)
	printf("%d", arr[i]);
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
    int arr1[200];
    for(int i=0; i<200; i++) arr1[i] = 0;

    generate(LEN, NUM_ONE, 1000, arr1);
    printf("generated string:\n");
    show_string(arr1);
    printf("\n\n");
    
}

void next_test()
{
    int arr1[200];
    for(int i=0; i<NUM_ONE; i++)
	arr1[i] = 1;

    // show first few strings
    for(int i=0; i<1050; i++)
    {
	if(i+1 == 1000)
	{
	    printf("string %d: ", i+1);
	    show_string(arr1);
	    printf("\n");
	}
	next(arr1);
    }
    printf("\n\n");
}

// test string
void test_string(int arr1[])
{
    int arr2[112];
    int comb = 112 / LEN; // koto gulo jora hobe
    int idx = 0;
    for(int i=0; i<LEN; i++)
	for(int j=0; j<comb; j++)
	    arr2[idx++] = arr1[i];

    long arr3[115]; idx = 0;
    for(int i=0; i<115; i++)
    {
	if(i == 0) arr3[i] = 0;
	else if(i == 93) arr3[i] = 1;
	else if(i == 114) arr3[i] = 0;
	else
	    arr3[i] = arr2[idx++];
    }

    // test string
    /* int sum = 0; */
    /* for(int i=0; i<115; i++) */
    /* { */
    /* 	printf("%d ", arr3[i]); */
    /* 	sum = sum + arr3[i]; */
    /* } */
    /* printf("sum = %d\n\n", sum); */

    int x = check1(arr3, C, b, c, n, m);
    if(x == 1)
    {
	printf("pass:\n");
	for(int i=0; i<115; i++)
	    printf("%d ", arr3[i]);
	printf("\n\n");
	exit(1);
    }
}

// test num strings starting
// from the kth string
void search(long k, long num)
{
    int arr1[200];
    for(int i=0; i<200; i++) arr1[i] = 0;
    generate(LEN, NUM_ONE, k, arr1);

    // printf("serach:\n");
    for(long i=0; i<num; i++)
    {
	/* printf("thread %d: ", omp_get_thread_num()); */
	/* show_string(arr1); */
	/* printf("\n"); */
	if((i & 0xfffff) == 0) 
	{
	    printf("thread %d: %ld / %ld\n", omp_get_thread_num(), i, num);
	    show_string(arr1);
	    printf("\n\n");
	}

	test_string(arr1);
	next(arr1);
    }
}

void start()
{
    total = combinations(LEN, NUM_ONE);
    long per_process = total / CORES + 1;

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

void start_openmp()
{
    total = combinations(LEN, NUM_ONE);
    int i;

    /* printf("total = %ld\n", total); */
    /* printf("per_process: %ld\n\n", total / CORES); */

    #pragma omp parallel
    {
	int id = omp_get_thread_num();
	long num_threads = omp_get_num_threads();
	long per_process = total/num_threads + 1;
	if(id == 0)
	{
	    real_num_threads = num_threads;
	    printf("num_threads: %ld\n", num_threads);
	    printf("per_process: %ld\n\n", per_process);
	}
	long k = id * per_process + 1;
	search(k, per_process);
    }
}

void init()
{
    for(int i=0; i<60; i++)
	for(int j=0; j<60; j++)
	    ncr[i][j] = -1;

    long tmp;
    for(int i=0; i<60; i++)
	for(int j=0; j<60; j++)
	    tmp = combinations(i, j);
}

int main()
{
    init();
    input_constraint(C, b, c, "constraint115.txt", &n, &m);
    // show_ilp(C, b, c, n ,m);

    /* next_test(); */
    /* generate_test(); */
    start_openmp();

    printf("\n\n  thikache\n\n");
    return 0;
}
