#include <stdio.h>


#define N 21
#define A 7
#define B 3

long bent;
long TARGET_NON_LINEARITY; 

long l_lower;
long l_upper;
long t_lower;
long t_upper;


long exp1(long x, int n)
{
    if(n == 1)
	return x;
    if(n == 0)
	return 1;
    long ans = 1;
    if(n % 2 == 1)
	ans = ans * x;
    ans = ans * exp1(x * x, n / 2);
    return ans;
}
long bent_concatenation_bound()
{
    long ans = exp1(2, N - 1)  - exp1(2, (N - 1) / 2);
    return ans;
}
void init()
{
    bent = bent_concatenation_bound();
    TARGET_NON_LINEARITY = bent + 161;
}


long calc_l()
{
    long d = exp1(2, A) - 1;
    l_lower = TARGET_NON_LINEARITY / d;

    if(TARGET_NON_LINEARITY %d != 0)
	l_lower++;

    l_upper = (exp1(2, N) - TARGET_NON_LINEARITY) / d;

    printf("%ld <= l <= %ld", l_lower, l_upper);
    printf("\n\n");
}

long calc_t()
{
    long d = exp1(2, A);

    t_lower = l_upper + TARGET_NON_LINEARITY - exp1(2, N - 1);
    t_lower = t_lower / d;
    if(t_lower % d != 0) t_lower++;

    t_upper = l_lower - TARGET_NON_LINEARITY + exp1(2, N - 1);
    t_upper = t_upper / d;


    printf("%ld <= t(alpha) <= %ld", t_lower, t_upper);
    printf("\n\n");
}


void test()
{
    printf("Target Non-linearity:\n");
    printf("2^(%ld - 1) - 2^(%ld - 1)/2 + %ld ", N, N, TARGET_NON_LINEARITY - bent);
    printf("= %ld\n\n", TARGET_NON_LINEARITY);

    calc_l();
    calc_t();
}

int main()
{
    init();
    test();

    printf("\n\n  thikache\n\n");
    return 0;
}
