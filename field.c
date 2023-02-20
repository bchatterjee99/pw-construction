#include <stdio.h>
#include "field.h"

// M := GF(2^15)
// L := GF(2^5)
// J := GF(2^3)
// K := GF(2)


unsigned int polynomial_addition(unsigned int a, unsigned int b)
{
    unsigned int ans = a ^ b;
    return ans;
}

unsigned int polynomial_multiplication(unsigned int a, unsigned int b)
{
    unsigned int tmp = b;
    unsigned int ans = 0;
    unsigned int shifted_a = (unsigned int)a;
    for(int i=0; i<SIZE; i++)
    {
	//printf("poly-mult, i: %d, shifted_a: %4x\n", i, shifted_a);
	if(tmp & 1)
	    ans = polynomial_addition(ans, shifted_a);
	shifted_a = shifted_a << 1;
	tmp = tmp >> 1;
    }
    return ans;
}	


unsigned int field_addition(unsigned int a, unsigned int b)
{
    unsigned int ans = a ^ b;
    return ans;
}

int deg(unsigned int a)
{
    int i = 2*SIZE - 1;
    while(i >= 0)
    {
	unsigned int tmp = (1 << i);
	if(a & tmp) return i;
	i--;
    }
    return 0;
}

unsigned int shift(unsigned int a, int s)
{
    unsigned int ans = a << s;
    return ans;
}

unsigned int modulo(unsigned int a)
{
    unsigned int primitive = PRIMITIVE; 
    // printf("modulo start\n");
    /* printf("deg(a): %d\n", deg(a)); */
    /* printf("deg(primitive): %d\n", deg(primitive)); */
    while(deg(a) >= deg(primitive))
    {
	//printf("modulo deg(a): %d\n", deg(a));
	a = polynomial_addition(a, shift(primitive, deg(a) - deg(primitive)));
    }
    return a;
}

unsigned int field_multiplication(unsigned int a, unsigned int b)
{
    unsigned int prod = polynomial_multiplication(a, b);
    // printf("polynomial mult: %x\n", prod);
    unsigned ans = modulo(prod);
    return ans;
}

unsigned int field_exponent(unsigned int a, int power)
{
    unsigned int ans = 1;
    while(power--)
	ans = field_multiplication(ans, a);
    return ans;
}
