#include <stdio.h>
#include <stdlib.h>

#include "storage.h"

long target;
long l;
long reduced_targets[10];
int num_targets;

#define N 21
#define A 7
#define B 3

#define L_MULT 127

#define LOWER 57
#define UPPER 72

/*

  table[i][j] :=
  No. of ways to make
  SUM[t = 0,1,..i]{ [(l - 2^a.k_t)]^2 * O_t } = j
  by choosing k_t in [LOWER, UPPER]

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

#define TABLE_SIZE 58900000
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

		// new_target divisible by remaining orbit size
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

// merge arr1[0..len1], arr2[0,...len2] into arr3[0,...len1+len2]
void merge(long arr1[], int len1, long arr2[], int len2, long arr3[])
{
    int i = 0, j = 0;
    long* tmp = (long*)malloc((len1 + len2)*sizeof(long));
    long idx = 0;
    while(i < len1 || j < len2)
    {
	if(i >= len1) tmp[idx++] = arr2[j++];
	else if(j >= len2) tmp[idx++] = arr1[i++];
	else if(arr1[i] <= arr2[j]) tmp[idx++] = arr1[i++];
	else tmp[idx++] = arr2[j++];
    }
    // copy to arr3[]
    for(int i=0; i<idx; i++)
	arr3[i] = tmp[i];
    free(tmp);
}
void merge_sort(long arr[], int len)
{
    if(len == 1)
	return;
    int mid = len / 2;
    merge_sort(arr, mid);
    merge_sort(arr + mid, len - mid);
    merge(arr, mid, arr + mid, len - mid, arr);
}

#define NUM_COINS 16
long coins[NUM_COINS];
long coin_map[NUM_COINS];
long caps[NUM_COINS];
void get_coins()
{
    for(long k=LOWER; k<=UPPER; k++)
    {
	coins[k - LOWER] = g(k);
	coin_map[k - LOWER] = coins[k - LOWER];
    }
    merge_sort(coins, NUM_COINS);
}
void show_coin_map()
{
    printf("coins:\n");
    printf("------\n");
    for(long k=0; k<NUM_COINS; k++)
	printf("%ld: %ld\n", k, coins[k]);
    printf("\n\n");

    printf("coin_map:\n");
    printf("---------\n");
    for(long k=0; k<NUM_COINS; k++)
	printf("%ld: %ld\n", k + LOWER, coin_map[k]);
    printf("\n\n");

    printf("caps:\n");
    printf("---------\n");
    for(long k=0; k<NUM_COINS; k++)
	printf("%ld: %ld\n", coins[k], TABLE_SIZE/coins[k]);
    printf("\n\n");
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
    for(int k=LOWER; k<=UPPER; k++)
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
    get_coins();
}


void copy_table(long* table, long* old_table, int len)
{
    for(int i=0; i<len; i++)
	old_table[i] = table[i];
}
void copy_byte_table(unsigned char* table, unsigned char* old_table, int len)
{
    for(int i=0; i<len; i++)
	old_table[i] = table[i];
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
#define BYTE_INF 255
void add2(unsigned char* a, unsigned char b)
{
    unsigned char remainder = BYTE_INF - *a;
    if(b > remainder)
	*a = BYTE_INF;
    else
	*a = *a + b;
}

#define NUM_POSSIBLE 17996735
void possible_sums()
{
    char* table = (char*)malloc(TABLE_SIZE * sizeof(char));
    char* old_table = (char*)malloc(TABLE_SIZE * sizeof(char));

    for(int i=0; i<TABLE_SIZE; i++)
    {
	old_table[i] = 0;
	table[i] = 0;
    }
    old_table[0] = 1;

    for(int i=0; i<112; i++)
    {
	long curr_possible = 0;
	for(long j=0; j<TABLE_SIZE; j++)
	{
	    /* if((j & 0xffffff) == 0) */
	    /* 	printf("i: %d  j: %ld\n", i, j); */

	    if(old_table[j] == 0) continue;
	    curr_possible++;

	    for(int k=0; k<NUM_COINS; k++)
		if(j + coins[k] < TABLE_SIZE)
		    table[j + coins[k]] = 1;
	}
	printf("coin %d  curr_possible: %ld\n", i-1, curr_possible);
	for(int j=0; j<TABLE_SIZE; j++)
	    old_table[j] = table[j];
    }

    long num_possible = 0;
    for(long j=0; j<TABLE_SIZE; j++)
    {
	// printf("%ld: %d\n", j, table[j] * 0xffffff);
	num_possible = num_possible + table[j];
    }
    printf("num_possible: %ld\n\n", num_possible);

    free(table);
    free(old_table);
}

void count1(long curr_target)
{
    char** table = (char**)malloc(113 * sizeof(char*));
    char** old_table = (char**)malloc(113 * sizeof(char*));
    long* old_possible = (long*)malloc(NUM_POSSIBLE * sizeof(long));
    long* possible = (long*)malloc(NUM_POSSIBLE * sizeof(long));
    int curr_possible = 0;

    for(int i=0; i<113; i++)
    {
	table[i] = (char*)malloc(NUM_POSSIBLE * sizeof(char));
	old_table[i] = (char*)malloc(NUM_POSSIBLE * sizeof(char));
    }

    // clear table
    old_possible[0] = 0;

    for(int c=0; c<NUM_COINS; c++)
    {
	for(int i=0; i<113; i++)
	{
	    for(int j=0; j<TABLE_SIZE; j++)
	    {
		
	    }
	}
    }


    for(int i=0; i<113; i++)
    {
	free(table[i]);
	free(old_table[i]);
    }
    free(table);
    free(old_table);
    free(possible);
    free(old_possible);
}

void unrestricted_num_coins()
{
    long *table = (long*)malloc(TABLE_SIZE * sizeof(long));
    long *old_table = (long*)malloc(TABLE_SIZE * sizeof(long));

    for(int j=0; j<TABLE_SIZE; j++)
    {
	table[j] = 0;
	old_table[j] = 0;
    }
    old_table[0] = 1;

    printf("unrestricted_num_coins(): start\n");

    for(int i=0; i<NUM_COINS; i++)
    {
	for(int j=0; j<TABLE_SIZE; j++)
	{
	    if((j & 0xfffff) == 0)
		printf("coin %d, TABLE_SIZE - j: %ld\n", i, TABLE_SIZE - j);

	    if(old_table[j] == 0) continue;

	    long curr = 0; int num_curr_coin = 0;
            // take at most 112 of each coin
	    while(curr + j < TABLE_SIZE && num_curr_coin < 112)
	    {
		table[j+curr] = table[j+curr] + old_table[j];
		curr = curr + coins[i];
		num_curr_coin++;
	    }
	}
	copy_table(table, old_table, TABLE_SIZE);
	for(int j=0; j<TABLE_SIZE; j++) table[j] = 0;
    }
    printf("\n\n");

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_targets[%d]: %ld, count: %ld\n",
	       i, reduced_targets[i], old_table[reduced_targets[i]]);
    }
}

void need_RAM()
{
    // table[c][i][s]
    // := number of ways to make s using i coins
    //    in {coin_0, coin_1, ... coin_c}
    //
    // table[c][i][s]
    // = SUM{k=0...i} table[c - 1][i - k][s - coins[c] * k]

    unsigned char **table = (unsigned char **)malloc(113 * sizeof(unsigned char*));
    unsigned char **old_table = (unsigned char **)malloc(113 * sizeof(unsigned char*));
    for(int i=0; i<=112; i++)
    {
	table[i] = (unsigned char *)malloc(TABLE_SIZE * sizeof(unsigned char));
	old_table[i] = (unsigned char *)malloc(TABLE_SIZE * sizeof(unsigned char));
    }

    long count = 0;
    for(int i=0; i<=112; i++)
    {
	printf("allocating memory: row %d/113\n", i);
	for(int j=0; j<TABLE_SIZE; j++)
	{
	    count++;
	    old_table[i][j] = 0;
	    // printf("%d", table[i][j]);
	}
    }
    printf("\n\n");
    old_table[0][0] = 1;
    printf("size: %ld\n", count/exp1(2, 30));

    for(int c=0; c<NUM_COINS; c++)
    {
	for(int i=0; i<=112; i++)
	{
	    printf("coin: %d, row: %d\n", c, i);
	    for(int s=0; s<TABLE_SIZE; s++)
	    {
		table[i][s] = 0;
		for(int k=0; k<=i; k++)
		{
		    long idx = (long)s -  ((long)k)*(coins[c]);
		    if(idx < 0) continue;
		    table[i][s] += old_table[i - k][idx];
		}
	    }
	}
	for(int i=0; i<=112; i++)
	    copy_byte_table(table[i], old_table[i], TABLE_SIZE);
    }
    printf("\n\n");

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_targets[%d]: %ld, count: %ld\n",
	       i, reduced_targets[i], old_table[reduced_targets[i]]);
    }


    for(int i=0; i<=112; i++)
    {
	free(table[i]);
	free(old_table[i]);
    }
    free(table);
    free(old_table);
}

void harddisk()
{
    printf("harddisk init:\n");

    for(int i=0; i<=112; i++)
    {
	printf("table 0 row %d\n", i);
	start_row(0, i);
	for(int j=0; j<TABLE_SIZE; j++)
	{
	    if(i == 0 && j == 0)
		set_cell(1);
	    else
		set_cell(0);
	}
	end_row();
    }
    printf("\n\n");

    for(int c=0; c<NUM_COINS; c++)
    {
	int old_table = c % 2;
	int table = 1 - (c % 2);
	for(int i=0; i<=112; i++)
	{

	    start_row(table, i);
	    for(int s=0; s<TABLE_SIZE; s++)
	    {
		if((s & 0xfffff) == 0)
		    printf("coin: %d row: %d TABLE_SIZE - s: %d\n",
			   c, i, TABLE_SIZE - s);

		int curr = 0;
		for(int k=0; k<=i; k++)
		{
		    int idx = s - k * coins[c];
		    if(idx < 0) continue;
		    curr = curr + get_cell(old_table, i - k, idx);
		}
		set_cell(curr);
	    }
	    end_row();
	}
    }
    printf("\n\n");

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_targets[%d]: %ld, count: %ld\n",
	       i, reduced_targets[i], get_cell(0, 112, reduced_targets[i]));
    }
}

void new_recurrence()
{

    // table[c][i][s]
    // := number of ways to make s using i coins
    //    in {coin_0, coin_1, ... coin_c}
    //
    // table[c][i][s]
    // = table[c][i - 1][s - coins[c]] + table[c-1][i][s] !!!

    unsigned char** table = (unsigned char **)malloc(NUM_COINS * sizeof(unsigned char *));
    unsigned char** old_table = (unsigned char **)malloc(NUM_COINS * sizeof(unsigned char *));
    for(int i=0; i<NUM_COINS; i++)
    {
	table[i] = (unsigned char *)malloc(TABLE_SIZE * sizeof(unsigned char));
	old_table[i] = (unsigned char *)malloc(TABLE_SIZE * sizeof(unsigned char));
    }

    // initial table (i = 0)
    for(int c=0; c<NUM_COINS; c++)
	for(int s=0; s<TABLE_SIZE; s++)
	    old_table[c][s] = 0;
    for(int c=0; c<NUM_COINS; c++)
	old_table[c][0] = 1;


    for(int i=1; i<=112; i++)
    {
	for(int c=0; c<NUM_COINS; c++)
	{
	    printf("i:%d, coin: %d\n", i, c);
	    table[c][0] = 1;
	    for(int s=1; s<TABLE_SIZE; s++)
	    {
		table[c][s] = 0;

		if(s - coins[c] >= 0)
		    // table[c][s] = table[c][s] + old_table[c][s - coins[c]];
		    add2(&table[c][s], old_table[c][s - coins[c]]);
		if(c > 0)
		    // table[c][s] = table[c][s] + table[c-1][s];
		    add2(&table[c][s], table[c-1][s]);
	    }
	}
	for(int c=0; c<NUM_COINS; c++)
	    copy_byte_table(table[c], old_table[c], TABLE_SIZE);
    }
    printf("\n\n");

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_targets[%d]: %ld, count: %ld\n",
	       i, reduced_targets[i], old_table[NUM_COINS-1][reduced_targets[i]]);
    }

    for(int i=0; i<NUM_COINS; i++)
    {
	free(table[i]);
	free(old_table[i]);
    }
    free(table);
    free(old_table);
}

int main()
{
    calc_target();
    show_coin_map();

    // possible_sums();
    printf("table-size: %ld\n\n", (TABLE_SIZE * (long)16 * (long)2) / exp1(2, 30));

    // need_RAM();
    // unrestricted_num_coins();
    new_recurrence();

    for(int i=0; i<num_targets; i++)
    {
	printf("reduced_target %d: %ld\n",
	       i, reduced_targets[i]);
    }

    printf("\n\n  thikache\n\n");
    return 0;
}
