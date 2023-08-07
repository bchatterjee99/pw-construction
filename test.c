#include <stdio.h>
#include <string.h>
#include <gmp.h>



int main()
{
    char *num = "00000000000000000000000000000000000001000000011110100110110111";
    // char* num = "000000000000000000000000000000000000010000000101101111111001";

    int len = strlen(num);
    printf("len = %d\n", len);

    mpz_t a;
    mpz_t t;
    mpz_t b;
    mpz_init(a);
    mpz_init(t);
    mpz_init(b);

    mpz_set_si(a, 0);
    for(int i=0; i<len; i++)
    {
	if(num[i] == '0') continue;
	mpz_ui_pow_ui(t, (long)2, (long)(114 - i));
	mpz_add(a, a, t);
    }
    printf("a: ");
    mpz_out_str(stdout, 10, a);
    printf("\n\n");

    mpz_sub_ui(a, a, 1);
    printf("a: ");
    mpz_out_str(stdout, 10, a);
    printf("\n\n");

    mpz_ui_pow_ui(t, (long)2, (long)115);
    // mpz_ui_pow_ui(t, (long)2, (long)10);
    // mpz_set_si(a, 10);
    mpz_fdiv_q(b, t, a);
    printf("b: ");
    mpz_out_str(stdout, 10, b);
    printf("\n\n");




    printf("\n\n");
    return 0;
}
