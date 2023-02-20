#include <math.h>
#include "draw.h"

#include "grid.h"

int cell_width;
int num_row; // number of rows
int num_col; // number of columns

void draw_grid(int num_cell)
{
    int tmp = SCREEN_WIDTH * SCREEN_HEIGHT / num_cell;
    cell_width = sqrt(tmp);
    printf("cell-width = %d\n\n", cell_width);
}
