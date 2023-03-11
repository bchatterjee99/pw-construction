#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>

long target;
long l;
long reduced_targets[10];
int num_targets;

#define N 21
#define A 7
#define B 3

#define L_MULT 127


/*

  table[i][j] :=
  No. of ways to make
  SUM[t = 0,1,..i]{ [(l - 2^a.k_t)]^2 * O_t } = j
  by choosing k_t in [57, 72]

  table[i][j] =
  SUM[t = 57,58,...72]{ table[i-1][j - f(k, i)] }

  f(k, i) := [(l - 2^a.k)]^2 * O_i

 */

long exp1(long a, int x)
{
    if(x == 0) return 1;
    if(x == 1) return a;
    long ans = 1;
    if(x & 1)
	ans = ans * a;
    ans = ans * exp1(a * a, x/2);
    return ans;
}

void calc_l()
{
    l = 56 * 147 + 21;
    printf("l = %ld\n\n", l);
}


// function f
long f(long k, int i)
{
    long ans = l - exp1(2, A) * k;
    ans = ans * ans;
    if(i == 0) ans = ans * 1;
    else if(i == 93 || i == 114) ans = ans * 3;
    else ans = ans * 21;
    return ans;
}

// function g
long g(long k)
{
    long ans = l - exp1(2, A) * k;
    ans = ans * ans;
    return ans;
}

void remove_small_orbit_rows()
{
    long arr1[] = {63, 66};
    long arr2[] = {59, 63, 66, 70};

    num_targets = 0;
    printf("remove_small_orbit_rows():\n");
    printf("--------------------------\n\n");
    long new_target;
    for(int i=0; i<2; i++)
    {
	for(int j=0; j<4; j++)
	{
	    for(int k=0; k<4; k++)
	    {
		new_target = target - f(arr1[i], 0) - f(arr2[j], 93) - f(arr2[k], 114);
		if(new_target % 21) continue;
		new_target = new_target / 21;

		printf("row-0: %ld, row-93: %ld, row-114: %ld\n",
		       arr1[i], arr2[j], arr2[k]);
		printf("new-target: %ld\n\n", new_target);

		// add new_target
		int flag = 0;
		for(int l=0; l<num_targets; l++)
		    if(reduced_targets[l] == new_target)
			flag = 1;
		if(flag == 0) reduced_targets[num_targets++] = new_target;
		    
	    }
	}
    }
}

void calc_target()
{
    calc_l();
    target = ((unsigned long)1 << (2 * N));
    long w_0 = exp1(2, N) - 2 * L_MULT * l;
    target = target - w_0 * w_0;

    if(target % L_MULT)
    {
	printf("calc_target(): 2^N - W_0^2 not divisible by L_MULT\n\n");
	exit(1);
    }
    target = target / L_MULT;

    // all orbit sizes divisible by 7
    if(target % 7)
    {
	printf("calc_target(): 2^N - W_0^2 / L_MULT not divisible by 7\n\n");
	exit(1);
    }
    target = target / 7;

    // all terms mult by 4
    if(target % 4)
    {
	printf("calc_target(): 2^N - W_0^2 / (L_MULT * 7) not divisible by 4\n\n");
	exit(1);
    }
    target = target / 4;

    printf("target = %ld\n\n", target);


    // find max f(k, i)
    long f = 0; long f_max = 0;
    for(int k=57; k<=72; k++)
    {
	f = (l - exp1(2, A) * k);
	// printf("f: %ld\n", f);
	f = f * f * 21;
	// printf("f-squared: %ld\n", f);
	if(f >= f_max) f_max = f;
    }
    printf("f_max = %ld\n", f_max);
    printf("stored-columns size: %ld GB\n\n", (f_max * (long)920)/exp1(2, 30));

    remove_small_orbit_rows();
}

void copy_table(long* table, long* old_table, int len)
{
    for(int i=0; i<len; i++)
	old_table[i] = table[i];
}
void copy_table_gmp(mpz_t* table, mpz_t* old_table, int len)
{
    for(int i=0; i<len; i++)
	mpz_set(old_table[i], table[i]);
}

#define INF 2305843009213693951
void add1(long* a, long b)
{
    long remainder = INF - *a;
    if(b > remainder)
	*a = INF;
    else
	*a = *a + b;
}

long table[58900000];
long old_table[58900000];
/* mpz_t table[58900000]; */
/* mpz_t old_table[58900000]; */
void count(long curr_target)
{
    for(int i=0; i<curr_target+1; i++)
    {
	/* mpz_init(table[i]); */
	/* mpz_init(old_table[i]); */
	/* mpz_set_si(old_table[i], (long)0); */
	old_table[i] = 0;
    }
    old_table[0] = 1;
    // mpz_set_si(old_table[0], (long)1);

    printf("count():\n");
    printf("--------\n\n");
    for(int i=0; i<115; i++)
    {
	for(long j=0; j<=curr_target; j++)
	{
	    if((j & 0xfffff) == 0)
		printf("i = %d,  curr_target - j = %ld\n", i, curr_target - j);

	    table[j] = 0;
	    // mpz_set_si(table[j], (long)0);
	    for(int k=57; k<=72; k++)
	    {
		long j1 = j - g(k);
		if(j1 < 0) continue;
		// table[j] += old_table[j1];
		add1(&table[j], old_table[j1]);
		// mpz_add(table[j], table[j], old_table[j1]);
	    }
	}
	copy_table(table, old_table, curr_target);
	// copy_table_gmp(table, old_table, curr_target);
    }
    printf("\n\n");

    printf("count:\n");
    for(long j=0; j<=curr_target; j++)
    {
	printf("count[%ld]: ", j);
	printf("%ld", table[j]);
	// mpz_out_str(stdout, 10, table[j]);
	printf("\n");
    }

    /* for(int i=0; i<=curr_target; i++) */
    /* { */
    /* 	mpz_clear(table[i]); */
    /* 	mpz_clear(old_table[i]); */
    /* } */
}

int main()
{
    calc_target();

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_target %d: %ld\n", i, reduced_targets[i]);
	printf("table-size: %ld\n\n", (reduced_targets[i] * (long)64) / exp1(2, 30));
	count(reduced_targets[i]);
    }

    printf("\n\n  thikache\n\n");
    return 0;
}
