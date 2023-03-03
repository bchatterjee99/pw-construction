#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


// ilp.c --------------------------------------------------------------
void input_constraint(long A[][200], long b[], long c[], const char* file, int* C_n, int* C_m)
{
    FILE* fp = fopen(file, "r");

    fscanf(fp, "%d", C_n);
    fscanf(fp, "%d", C_m);

    for(int i=0; i<*C_n; i++)
    {
	for(int j=0; j<*C_m; j++)
	    fscanf(fp, "%d", &A[i][j]);
    }

    for(int i=0; i<*C_n; i++)
	fscanf(fp, "%d", &b[i]);


    for(int i=0; i<*C_n; i++)
	fscanf(fp, "%d", &c[i]);
}
void show_ilp(long C[][200], long b[], long c[], int n, int m)
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
void mult_vector(long A[][200], long b[], long c[], int n)
{
    for(int i=0; i<n; i++)
    {
	    c[i] = 0;
	    for(int k=0; k<n; k++)
		c[i] = c[i] + A[i][k] * b[k];
    }
}
int check1(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m)
{
    int flag = 0;
    for(int i=0; i<C_n; i++)
    {
	long tmp = 0;
	for(int j=0; j<C_m; j++)
	    tmp = tmp + f[j] * Constraint[i][j];
	if(tmp < b[i] || tmp > c[i])
	{
	    /* printf("constraint %d failed\n", i); */
	    /* printf("b = %ld, tmp = %ld, c = %ld\n\n", b[i], tmp, c[i]); */
	    flag = 1;
	    return 0; // return on first fail
	}
    }
    if(flag == 0) return 1;
    return 0;
}
// ilp.c --------------------------------------------------------------


long C[200][200];
long b[200];
long c[200];
int n;
int m;

// generate LEN length strings
// with NUM_ONE 1's
// ...
// Combinatorial Number System of degree NUM_ONE
#define LEN 56
#define NUM_ONE 28

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
// ...
// kth number in Combinatorial Number System of degree NUM_ONE
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
// ...
// next number in Combinatorial Number System of degree NUM_ONE
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

    // expand to 112
    for(int i=0; i<LEN; i++)
	for(int j=0; j<comb; j++)
	    arr2[idx++] = arr1[i];

    // fill gap
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
	FILE* fp = fopen("found.txt", "w");
	printf("pass:\n");
	for(int i=0; i<115; i++)
	{
	    printf("%d ", arr3[i]);
	    fprintf(fp, "%d ", arr3[i]);
	}
	printf("\n\n");
	fprintf(fp, "\n\n");
	fclose(fp);
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
    long per_thread = total / CORES + 1;

    int i;
    for(i=0; i<CORES; i++)
    {
	int pid = fork();
	if(pid != 0)
	    break;
    }
    if(i == CORES) exit(1);
    printf("process %d: i = %d\n", getpid(), i);

    search(i * per_thread + 1, per_thread);

    int status;
    while(wait(&status) > 0); // wait for child process
    printf("process %d end\n", getpid());
}

void start_openmp()
{
    total = combinations(LEN, NUM_ONE);
    int i;

    /* printf("total = %ld\n", total); */
    /* printf("per_thread: %ld\n\n", total / CORES); */

    #pragma omp parallel
    {
	int id = omp_get_thread_num();
	long num_threads = omp_get_num_threads();
	long per_thread = total/num_threads + 1;
	if(id == 0)
	{
	    real_num_threads = num_threads;
	    printf("num_threads: %ld\n", num_threads);
	    printf("per_thread: %ld\n\n", per_thread);
	}
	long k = id * per_thread + 1;
	search(k, per_thread);
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
