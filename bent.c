#include <stdio.h>

long N;
#define ROOT2 1.41421356237

long exp1(long a, long b)
{
    long ans = 1;
    while(b > 0)
    {
	if(b & 1)
	    ans = ans * a;
	a = a * a;
	b = b >> 1;
    }
    return ans;
}

double upper_bound()
{
    double ans = (double) exp1(2, N-1);
    double tmp = (double) exp1(2, N/2 - 1) * ROOT2;
    ans = ans - tmp;
    return ans;
}

// bent concatenation bound
long bent_concatenation()
{
    long ans = exp1(2, N-1) - exp1(2, (N-1)/2);
    return ans;
}

// evaluate linear boolean function
int eval_linear(unsigned long func, unsigned long input)
{
    int ans = 0;
    while(func > 0)
    {
	ans = ans ^ ((func & input) & 1);
	func = func >> 1;
	input = input >> 1;
    }
    return ans;
}

// calc non-linearity
long non_linearity(unsigned long func)
{
    unsigned long U = ((unsigned long)1 << N);
    long min_dist = U;
    unsigned long nearest = 0;
    char affine = 0;

    for(unsigned long a=0; a<U; a++)
    {
	unsigned long tmp_func = func;
	int dist = 0;
	for(unsigned long e=0; e<U; e++)
	{
	    int val = eval_linear(a, e);
	    int bit = tmp_func & 1;
	    if(val != bit)
		dist++;
	    tmp_func = tmp_func >> 1;
	}
	if(dist < min_dist)
	{
	    min_dist = dist;
	    nearest = a;
	    affine = 0;
	}

	// printf("%d, %d,  ", dist, U - dist);

	// complement affine
	dist = U - dist;
	if(dist < min_dist)
	{
	    min_dist = dist;
	    nearest = a;
	    affine = 1;
	}
    }

    /* printf("nearest: %b\n", nearest); */
    /* printf("affine: %d\n", affine); */
    /* printf("non-linearity: %d\n", min_dist); */
    return min_dist;
}

void show_func(unsigned long func)
{
    printf("function:\n");
    unsigned long U = ((unsigned long)1 << N);
    for(unsigned long e=0; e<U; e++)
    {
	unsigned long tmp = ((unsigned long)1) << (N-1);
	for(int i=0; i<N; i++)
	{
	    printf("%d", !!(tmp & e));
	    tmp = tmp >> 1;
	}
	printf(" | ");
	printf("%d", func & 1);
	printf("\n");
	func = func >> 1;
    }
    printf("\n\n");
}

void show_linear_funcitons()
{
    unsigned long U = ((unsigned long)1 << N);
    long min_dist = U;
    unsigned long nearest = 0;

    for(unsigned long a=0; a<U; a++)
    {
	printf("linear-function:\n");
	unsigned long U = ((unsigned long)1 << N);
	for(unsigned long e=0; e<U; e++)
	{
	    unsigned long tmp = ((unsigned long)1) << (N-1);
	    for(int i=0; i<N; i++)
	    {
		printf("%d", !!(tmp & e));
		tmp = tmp >> 1;
	    }
	    printf(" | ");
	    printf("%d", eval_linear(a, e));
	    printf("\n");
	}
	printf("\n\n");
    }
}

void search()
{
    unsigned long size = ((unsigned long)1) << N;
    unsigned long num_func = ((unsigned long)1) << size;
    printf("N: %ld, size: %ld, num_func: %ld\n", N, size, num_func);
    printf("bent catenation bound: %ld\n", bent_concatenation());
    long max_nl = 0, nl; long nl_func = 0;
    for(int f=0; f<num_func; f++)
    {
	nl = non_linearity(f);
	if(nl > max_nl)
	{
	    max_nl = nl;
	    nl_func = f;
	}
    }
    printf("maximum non linearity: %ld\n", max_nl);
    printf("maximum non linear function\n");
    show_func(nl_func);
    show_linear_funcitons();
    printf("\n\n");
}

int main()
{
    N = 7;
    printf("N = %ld\n", N);
    printf("upper bound: %lf\n", upper_bound());
    printf("bent concatenation: %ld\n", bent_concatenation());
    printf("\n\n");

    N = 9;
    printf("N = %ld\n", N);
    printf("upper bound: %lf\n", upper_bound());
    printf("bent concatenation: %ld\n", bent_concatenation());
    printf("\n\n");

    N = 15;
    printf("N = %ld\n", N);
    printf("upper bound: %lf\n", upper_bound());
    printf("bent concatenation: %ld\n", bent_concatenation());
    printf("\n\n");

    N = 21;
    printf("N = %ld\n", N);
    printf("upper bound: %lf\n", upper_bound());
    printf("bent concatenation: %ld\n", bent_concatenation());
    printf("\n\n");
    return 0;
}
