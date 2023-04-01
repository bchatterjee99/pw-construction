#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define NUM_ROWS 113
#define NUM_COLUMNS 58900000


void to_str(int i, char* str)
{
    str[0] = ((i/100) % 10) + '0';
    str[1] = ((i/10) % 10) + '0';
    str[2] = (i % 10) + '0';
}

unsigned char get_cell(int row, int col)
{
    char* tmp = "./storage/000.dat"; int ret;
    char filename[18]; strcpy(filename, tmp);
    to_str(row, filename + 10);

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

void set_cell(int row, int col, unsigned char data)
{
    char* tmp = "./storage/000.dat"; int ret;
    char filename[18]; strcpy(filename, tmp);
    to_str(row, filename + 10);

    FILE* fp = fopen(filename, "w");
    if(fp == NULL) { printf("set_cell() error: could not open file: %s\n", filename); }

    ret = fseek(fp, col, SEEK_SET);
    if(ret != 0)
	printf("get_cell() error, ret = %d\n", ret);

    errno = 0;
    ret = fputc(data, fp);

    if(ret != data)
	printf("set_cell() error, ret != data\n", errno);
    if(errno != 0)
	printf("set_cell() error, errno: %d\n", errno);


    fflush(fp);
    fclose(fp);
}

void storage_init()
{
    char* tmp = "./storage/000.dat";
    char filename[18]; strcpy(filename, tmp);
    for(int i=0; i<NUM_ROWS; i++)
    {
	to_str(i, filename + 10);
	printf("filename: %s\n", filename);
	FILE* fp = fopen(filename, "w");
	for(int j=0; j<NUM_COLUMNS; j++)
	    fputc(j & 0xff, fp);
	fclose(fp);
    }
}

#define TEST 100
int main()
{
    srand(time(0));
    // storage_init();
    int test_row[TEST], test_col[TEST];
    int expected[TEST];

    printf("storing:\n");
    for(int i=0; i<TEST; i++)
    {
	int row = rand() % NUM_ROWS;
	int col = (rand()) % NUM_COLUMNS;
	test_row[i] = row; test_col[i] = col;
	unsigned char data = rand() & 0xff;
	set_cell(row, col, data);
	expected[i] = data;
	// printf("row: %d, col: %d, val: %d\n", row, col, data);
    }
    printf("\n\n");

    printf("reading:\n");
    for(int i=0; i<TEST; i++)
    {
	int row = test_row[i];
	int col = test_col[i];
	unsigned char data;
	data = get_cell(row, col);
	// printf("row: %d, col: %d, val: %d, expected: %d\n", row, col, data, expected[i]);
    }
    printf("\n\n");


    printf("error:\n");
    printf("------\n");
    for(int i=0; i<TEST; i++)
    {
	int row = test_row[i];
	int col = test_col[i];
	unsigned char data;
	data = get_cell(row, col);
	if(data == expected[i]) continue;
	printf("row: %d, col: %d, val: %d, expected: %d\n", row, col, data, expected[i]);
    }
    printf("\n\n");
}
