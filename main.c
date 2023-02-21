#include <stdio.h>

#include "field.h"
#include "draw.h"
#include "grid.h"
#include "plane.h"

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
    /* find_LJ_mult(); */
    /* find_Points(); */
    /* find_Orbits(); */
    // plane_draw_Orbits();
    /* plane_draw_Orbits_grouped(); */
    /* plane_draw_line(0xff); */
    /* draw_update(); */
    plane_animate_random_lines();
    char ch = fgetc(stdin);
    destroy_plane();

    printf("\n\n  thikache\n\n");
    return 0;
}
