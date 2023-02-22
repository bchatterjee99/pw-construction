#include <stdio.h>

#include "field15.h"
#include "draw.h"
#include "grid.h"
#include "plane15.h"

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

void grid_test()
{
    draw_init();
    grid_draw(1057);
    grid_fill_cell(1056,0xff0000);
    draw_update();
    char ch = fgetc(stdin);
}

int main()
{
    draw_init();
    create_plane();
    // plane_animate_random_lines();
    plane_animate_Orbit_lines(2);
    destroy_plane();

    printf("\n\n  thikache\n\n");
    return 0;
}
