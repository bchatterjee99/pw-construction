#include <stdio.h>
#include <stdlib.h>

long targets[4];
long coins[16];

void init()
{
    targets[0] = 58818032;
    targets[1] = 58751728;
    targets[2] = 58882800;
    targets[3] = 58816496;
    
    coins[0] = 3721;
    coins[1] = 4489;
    coins[2] = 35721;
    coins[3] = 38025;
    coins[4] = 100489;
    coins[5] = 104329;
    coins[6] = 198025;
    coins[7] = 203401;
    coins[8] = 328329;
    coins[9] = 335241;
    coins[10] = 491401;
    coins[11] = 499849;
    coins[12] = 687241;
    coins[13] = 697225;
    coins[14] = 915849;
    coins[15] = 927369;
}


#define TABLE_SIZE 58900000
void possible_sums()
{
    unsigned char* table = (unsigned char*)malloc(TABLE_SIZE * sizeof(unsigned char));
    unsigned char* old_table = (unsigned char*)malloc(TABLE_SIZE * sizeof(unsigned char));

    for(int i=0; i<TABLE_SIZE; i++) old_table[i] = 0;
    old_table[0] = 1;

    for(int i=1; i<=112; i++)
    {
	for(int j=0; j<TABLE_SIZE; j++)
	{
	    if(old_table[j] == 0) continue;
	    for(int k=0; k<15; k++)
	    {
		int idx = j + coins[k];
		if(idx < TABLE_SIZE)
		    table[idx] = 1;
	    }
	}
	for(int j=0; j<TABLE_SIZE; j++)
	{
	    old_table[j] = table[j];
	    table[j] = 0;
	}

	printf("take %d: ", 112 - i);
	for(int j=0; j<4; j++)
	{
	    int new_target = targets[j] - (112 - i) * coins[15];
	    if(new_target >= 0) printf("%d", old_table[new_target]);
	    else printf("0");
	}
	printf("\n");
	fflush(stdout);
    }
}

// check coin i
int check(int i)
{
    int tmp = coins[i];
    coins[i] = coins[15];
    coins[15] = tmp;
    printf("checkingt coin %d: %ld\n", i, coins[15]);
    possible_sums();
}

int main()
{
    init();
    for(int i=0; i<16; i++)
	check(i);
    return 0;
}
