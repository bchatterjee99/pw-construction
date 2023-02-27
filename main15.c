#include <stdio.h>

#include "field15.h"
#include "graphics.h"
#include "grid.h"
#include "plane15.h"

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

int main()
{
    graphics_init();
    create_plane();
    /* plane_draw_Orbits_grouped(); */
    /* draw_update(); */
    /* char ch = fgetch(stdin); */
    // plane_animate_random_lines();
    plane_animate_Orbit_lines(2);

    graphics_end();
    destroy_plane();

    printf("\n\n  thikache\n\n");
    return 0;
}
