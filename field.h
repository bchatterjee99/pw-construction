

// M := GF(2^15)
// L := GF(2^5)
// J := GF(2^3)
// K := GF(2)


#define SIZE 15
#define PRIMITIVE 0x8003 // x^15 + x + 1


unsigned int polynomial_addition(unsigned int a, unsigned int b);

unsigned int polynomial_multiplication(unsigned int a, unsigned int b);

unsigned int field_addition(unsigned int a, unsigned int b);

int deg(unsigned int a);

unsigned int shift(unsigned int a, int s);

unsigned int modulo(unsigned int a);

unsigned int field_multiplication(unsigned int a, unsigned int b);

unsigned int field_exponent(unsigned int a, int power);
