#include <stdio.h>
#include <math.h>

#include "graphics.h"
#include "grid.h"

int cell_size;
int num_row; // number of rows
int num_col; // number of columns

#define OFF 2

int calc_grid_size(int cell_size)
{
    int ans = WINDOW_WIDTH / cell_size;
    ans = ans * (WINDOW_HEIGHT / cell_size);
    return ans;
}

int calc_cell_size(int num_cell)
{
    int l = 0, mid = 0, r = WINDOW_WIDTH; 
    if(WINDOW_HEIGHT < WINDOW_WIDTH) r = WINDOW_HEIGHT;
    while(l <= r)
    {
	mid = r - (r - l)/2;
	if(l == r) break;
	int grid_size = calc_grid_size(mid);
	if(grid_size >= num_cell)
	    l = mid;
	else
	    r = mid - 1;
    }
    return mid;
}

void grid_draw(int num_cell)
{
    cell_size = calc_cell_size(num_cell);
    num_row = WINDOW_HEIGHT / cell_size;
    num_col = WINDOW_WIDTH / cell_size;
    printf("cell-size = %d  num-row = %d  num-col= %d\n\n", cell_size, num_row, num_col);

    graphics_reset(0);
    graphics_set_color(10, 10, 10);
    int count = 0;
    int cell_x, cell_y, cell_w, cell_h;
    for(int i=0; i<num_row; i++)
    {
	for(int j=0; j<num_col; j++)
	{
	    cell_x = j * cell_size + OFF;
	    cell_y = i * cell_size + OFF;
	    cell_w = cell_size - 2 * OFF;
	    cell_h = cell_size - 2 * OFF;
	    graphics_draw_rect(cell_x, cell_y, cell_w, cell_h);

	    count++;
	    if(count == num_cell) break;
	}
	if(count == num_cell) break;
    }
}

void grid_fill_cell(int pos, int color)
{
    int i = pos / num_col; int j = pos % num_col; 
    int cell_x, cell_y, cell_w, cell_h;
    graphics_set_color( (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
    cell_x = j * cell_size + OFF;
    cell_y = i * cell_size + OFF;
    cell_w = cell_size - 2 * OFF;
    cell_h = cell_size - 2 * OFF;
    graphics_draw_rect(cell_x, cell_y, cell_w, cell_h);
}

void grid_mark_cell(int pos)
{
    int i = pos / num_col; int j = pos % num_col; 
    graphics_set_color(0, 0, 0);
    int startx = j * cell_size + OFF;
    int starty = i * cell_size + OFF;
    int endx = (j+1) * cell_size - OFF - 1;
    int x = 0, y = 0;
    int cell_x, cell_y, cell_w, cell_h;
    while(x <= cell_size - 4 * OFF + 1)
    {
	cell_x = startx + x;
	cell_y = starty + y;
	cell_w = OFF * 2;
	cell_h = OFF * 2;
	graphics_draw_rect(cell_x, cell_y, cell_w, cell_h);

	cell_x = endx - x - 2 * OFF + 1;
	cell_y = starty + y;
	cell_w = OFF * 2;
	cell_h = OFF * 2;
	graphics_draw_rect(cell_x, cell_y, cell_w, cell_h);

	x = x + 2 * OFF; y = y + 2 * OFF;
    }
}

void grid_update()
{
    graphics_update();
}
