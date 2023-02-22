#include <stdio.h>
#include "field15.h"

// M := GF(2^15)
// L := GF(2^5)
// J := GF(2^3)
// K := GF(2)

#define SIZE 15
#define PRIMITIVE 0x8003 // x^15 + x + 1


unsigned long polynomial_addition(unsigned long a, unsigned long b)
{
    unsigned long ans = a ^ b;
    return ans;
}

unsigned long polynomial_multiplication(unsigned long a, unsigned long b)
{
    unsigned long tmp = b;
    unsigned long ans = 0;
    unsigned long shifted_a = (unsigned long)a;
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


unsigned long field_addition(unsigned long a, unsigned long b)
{
    unsigned long ans = a ^ b;
    return ans;
}

int deg(unsigned long a)
{
    int i = 2*SIZE - 1;
    while(i >= 0)
    {
	unsigned long tmp = (1 << i);
	if(a & tmp) return i;
	i--;
    }
    return 0;
}

unsigned long shift(unsigned long a, int s)
{
    unsigned long ans = a << s;
    return ans;
}

unsigned long modulo(unsigned long a)
{
    unsigned long primitive = PRIMITIVE; 
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

unsigned long field_multiplication(unsigned long a, unsigned long b)
{
    unsigned long prod = polynomial_multiplication(a, b);
    // printf("polynomial mult: %x\n", prod);
    unsigned ans = modulo(prod);
    return ans;
}

unsigned long field_exponent(unsigned long a, int power)
{
    unsigned long ans = 1;
    /* while(power--) */
    /* 	ans = field_multiplication(ans, a); */
    while(power > 0)
    {
	if(power & 1)
	    ans = field_multiplication(ans, a);
	a = field_multiplication(a, a);
	power = power >> 1;
    }
    return ans;
}
