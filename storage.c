#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define NUM_ROWS 113
#define NUM_COLUMNS 58900000


void to_str(int i, char* str)
{
    str[0] = ((i/100) % 10) + '0';
    str[1] = ((i/10) % 10) + '0';
    str[2] = (i % 10) + '0';
}

unsigned char get_cell(int table, int row, int col)
{
    char filename[19];
    int ret;
    if(table == 0)
    {
	char* tmp = "./storage0/000.dat";
	strcpy(filename, tmp);
    }
    else
    {
	char* tmp = "./storage1/000.dat";
	strcpy(filename, tmp);
    }
    to_str(row, filename + 11);

    FILE* fp = fopen(filename, "r");
    if(fp == NULL) { printf("get_cell() error: could not open file: %s\n", filename); }

    ret = fseek(fp, col, SEEK_SET);
    if(ret != 0)
	printf("get_cell() error, ret = %d\n", ret);

    errno = 0;
    unsigned char data = fgetc(fp);

    if(errno != 0)
	printf("get_cell() error, errno: %d\n", errno);

    fflush(fp);
    fclose(fp);

    return data;
}

FILE* set_cell_fp;
void start_row(int table, int row)
{
    char filename[19];
    int ret;
    if(table == 0)
    {
	char* tmp = "./storage0/000.dat";
	strcpy(filename, tmp);
    }
    else
    {
	char* tmp = "./storage1/000.dat";
	strcpy(filename, tmp);
    }
    to_str(row, filename + 11);
    set_cell_fp = fopen(filename, "w");
    if(set_cell_fp == NULL) { printf("set_cell() error: could not open file: %s\n", filename); }
}

void set_cell(unsigned char data)
{
    fputc(data, set_cell_fp);
}

void end_row()
{
    fclose(set_cell_fp);
}

#define TEST 100
int test()
{
    srand(time(0));
    // storage_init();
    // return 0;

    int test_row[TEST], test_col[TEST];
    unsigned char expected[TEST];

    printf("storing:\n");
    start_row(0, 1);
    for(int i=0; i<TEST; i++)
    {
	unsigned char data = rand() & 0xff;
	set_cell(data);
	expected[i] = data;
	printf("row: %d, col: %d, val: %d\n", 1, i, data);
    }
    end_row();
    printf("\n\n");


    printf("error:\n");
    printf("------\n");
    for(int i=0; i<TEST; i++)
    {
	int row = 1;
	int col = i;
	printf("row: %d, col: %d, ", row, col);
	unsigned char data;
	data = get_cell(0, row, col);

	if(data == expected[i]) { printf("\n"); continue; }
	printf("val: %d, expected: %d\n", data, expected[i]);
    }
    printf("\n\n");
}

/* int main() */
/* { */
/*     test(); */
/*     return 0; */
/* } */
