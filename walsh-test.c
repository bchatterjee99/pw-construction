#include <stdio.h>
#include "ilp.h"


long C[200][200]; // constraint matrix
int n;



int main()
{
    long b[] = {0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0};
    long c[200];

    input_matrix(C, "C.txt", &n);
    mult_vector(C, b, c, n);
    show_vector(c, n, "A*b: ");

    long T21 = ((long) 1) << 21;
    long T42 = ((long) 1) << 42;

    // calc l
    long l = 0;
    for(int i=0; i<n; i++)
    {
	if(i == 0) l = l + b[i] * 1 * 7;
	else if(i == 93 || i == 114) l = l + b[i] * 3 * 7;
	else l = l + b[i] * 21 * 7;
    }
    printf("l = %ld\n\n", l);

    // calc spectrum sum
    long ans = 0;
    for(int i=0; i<n; i++)
    {
	long w_coeff = 2 * (l - 128 * c[i]);
	long w_coeff_square = w_coeff * w_coeff;

	// mult orbit * coset size
	long tmp = w_coeff_square * 127;
	if(i == 0)
	{
	    tmp = tmp * 1 * 7;   
	}
	else if(i == 93 || i == 114)
	{
	    tmp = tmp * 3 * 7;
	}
	else
	{
	    tmp = tmp * 21 * 7;
	}
	ans = ans + tmp;
	// printf("i = %d  num_func: %ld\n", i, num_func);
    }
    // add coeff for f(x) = 0
    long agreements = T21 - l * 127;
    long w_coeff = 2 * agreements - T21;
    long w_coeff_square = w_coeff * w_coeff;
    ans = ans + w_coeff_square;

    printf("ans = %ld\n", ans);
    printf("2^42 = %ld\n", T42);
    printf("\n\n");

    long num_func = 112 * 21 * 127 + 2 * 3 * 127 + 127;
    printf("num_func = %ld\n", num_func * 7);
    printf("2^21 = %ld\n", T21);
    printf("\n\n");

    printf("Case 15: %ld\n", 10 * (105 * 31) + 7 * 31);
    printf("2^15: %ld\n", ((long)1) << 15);
    printf("\n\n");

    printf("\n\n  thikache\n\n");
    return 0;
}
