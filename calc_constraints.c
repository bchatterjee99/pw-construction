#include <stdio.h>

/* #define N 15 */
/* #define A 5 */

#define N 21
#define A 7

#define T 1
long target;

long l_lower, l_upper;
long t_lower, t_upper;

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

// bent concatenation bound
long bent_concatenation()
{
    long ans = exp1(2, N-1) - exp1(2, (N-1)/2);
    return ans;
}

void set_target()
{
    target = bent_concatenation() + T;
}

void l_bounds()
{
    double lower = (double) target / (double) (exp1(2, A) - 1);
    double upper = (double) (exp1(2, N) - target) / (double) (exp1(2, A) - 1);
    printf("%lf <= l <= %lf\n", lower, upper);
}

void t_bounds()
{
    long lower = target * (exp1(2, A)  - 2);
    lower = lower + exp1(2, N);
    lower = lower - exp1(2, N - 1) * (exp1(2, A) - 1);
    double flower = (double) lower / (double) ( (exp1(2, A) * (exp1(2, A) - 1)) );

    long upper = exp1(2, N - 1) * (exp1(2, A) - 1);
    upper = upper - target * (exp1(2, A) - 2);
    double fupper = (double) upper / (double) ( (exp1(2, A) * (exp1(2, A) - 1)) );

    printf("%lf <= t <= %lf\n", flower, fupper);
}

int main()
{
    set_target();
    l_bounds();
    t_bounds();

    printf("aaaaa:\n");
    printf("%ld\n", 8249 / 147);
    printf("%ld\n", 8264 / 147);
    printf("%ld\n", 147 * 56);
    printf("%ld\n", 147 * 55);
    printf("%ld\n", 147 * 57);
    printf("\n\n");

    printf("\n\n");
    return 0;
}
