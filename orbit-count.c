#include <stdio.h>

#include "field15.h"

#define N 15
#define A 5
#define B 3

/* #define N 21 */
/* #define A 7 */
/* #define B 3 */

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

long gcd1(long a, long b)
{
    if(b == 0) return a;
    if(a == 0) return b;
    if(a == 1) return 1;
    if(b == 1) return 1;
    long d = a % b;
    long ans = gcd1(b, d);
    return ans;
}

long M_mult, L_mult, J_mult;
long L_power, J_power;

int main()
{
    M_mult = exp1(2, N) - 1;
    L_mult = exp1(2, A) - 1;
    J_mult = exp1(2, B) - 1;

    printf("M_mult: %ld, L_mult: %ld, J_mult: %ld\n\n", M_mult, L_mult, J_mult);

    L_power = M_mult / L_mult;
    J_power = M_mult / J_mult;

    printf("L_power: %ld, J_power: %ld\n\n", L_power, J_power);

    printf("gcd : %ld\n\n", gcd1(L_power, J_power));


    printf("\n  thikache\n\n");
    return 0;
}
