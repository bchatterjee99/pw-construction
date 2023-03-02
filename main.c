#include <stdio.h>

#include "field21.h"
#include "graphics.h"
#include "grid.h"
#include "world.h"
#include "plane21.h"

void field_test()
{
    unsigned int a = 0x09;
    unsigned int b = 0x1001;
    unsigned int c = field_multiplication(a, b);
    printf("c = 0x%x\n", c);
}

void grid_test()
{
    graphics_init();
    grid_draw(1057);
    grid_fill_cell(1056,0xff0000);
    graphics_update();
    char ch = fgetc(stdin);
}

void grahpics_circle_test()
{
    graphics_reset(0x000020);
    graphics_set_color(0, 0, 0);
    graphics_midpoint_circle(1, 1, 20);
    graphics_set_color(255, 0, 0);
    graphics_flood_fill(1, 1);
    graphics_update();
}

int main()
{
    graphics_init();
    create_plane();
    // construct_ILP();
    /* plane_draw_Orbits_grouped(); */
    /* draw_update(); */
    /* plane_animate_random_lines(); */
    /* plane_animate_Orbit_lines(2); */
    /* destroy_plane(); */

    // world_test();


    // char ch = fgetc(stdin);
    graphics_end();

    printf("\n\n  thikache\n\n");
    return 0;
}
