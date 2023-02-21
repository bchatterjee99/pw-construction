

// M := GF(2^15)
// L := GF(2^5)
// J := GF(2^3)
// K := GF(2)




unsigned long polynomial_addition(unsigned long a, unsigned long b);

unsigned long polynomial_multiplication(unsigned long a, unsigned long b);

unsigned long field_addition(unsigned long a, unsigned long b);

int deg(unsigned long a);

unsigned long shift(unsigned long a, int s);

unsigned long modulo(unsigned long a);

unsigned long field_multiplication(unsigned long a, unsigned long b);

unsigned long field_exponent(unsigned long a, int power);
