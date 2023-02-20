#include <stdio.h>

#include "field.h"
#include "draw.h"
#include "grid.h"

void field_test()
{
    unsigned int a = 0x09;
    unsigned int b = 0x1001;
    unsigned int c = field_multiplication(a, b);
    printf("c = 0x%x\n", c);
}

void draw_test()
{
    draw_init();
    char ch = fgetc(stdin);
    draw_end();
}

int main()
{
    draw_init();
    draw_grid(100);

    char ch = fgetc(stdin);
    printf("\n\n  thikache\n\n");
    return 0;
}
