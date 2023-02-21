#include <math.h>
#include "draw.h"

#include "grid.h"

int cell_size;
int num_row; // number of rows
int num_col; // number of columns

#define OFF 2

int calc_grid_size(int cell_size)
{
    int ans = SCREEN_WIDTH / cell_size;
    ans = ans * (SCREEN_HEIGHT / cell_size);
    return ans;
}

int calc_cell_size(int num_cell)
{
    int l = 0, mid = 0, r = SCREEN_WIDTH; 
    if(SCREEN_HEIGHT < SCREEN_WIDTH) r = SCREEN_HEIGHT;
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
    num_row = SCREEN_HEIGHT / cell_size;
    num_col = SCREEN_WIDTH / cell_size;
    printf("cell-size = %d  num-row = %d  num-col= %d\n\n", cell_size, num_row, num_col);

    draw_reset();
    draw_set_color(10, 10, 10);
    int count = 0;
    SDL_Rect cell;
    for(int i=0; i<num_row; i++)
    {
	for(int j=0; j<num_col; j++)
	{
	    cell.x = j * cell_size + OFF;
	    cell.y = i * cell_size + OFF;
	    cell.w = cell_size - 2 * OFF;
	    cell.h = cell_size - 2 * OFF;
	    SDL_RenderFillRect(renderer, &cell);
	    count++;
	    if(count == num_cell) break;
	}
	if(count == num_cell) break;
    }
}

void grid_fill_cell(int pos, int color)
{
    int i = pos / num_col; int j = pos % num_col; 
    SDL_Rect cell;
    draw_set_color( (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
    cell.x = j * cell_size + OFF;
    cell.y = i * cell_size + OFF;
    cell.w = cell_size - 2 * OFF;
    cell.h = cell_size - 2 * OFF;
    SDL_RenderFillRect(renderer, &cell);
}

void grid_mark_cell(int pos)
{
    int i = pos / num_col; int j = pos % num_col; 
    draw_set_color(0, 0, 0);
    int startx = j * cell_size + OFF;
    int starty = i * cell_size + OFF;
    int endx = (j+1) * cell_size - OFF - 1;
    int x = 0, y = 0;
    SDL_Rect cell;
    while(x <= cell_size - 4 * OFF + 1)
    {
	cell.x = startx + x;
	cell.y = starty + y;
	cell.w = OFF * 2;
	cell.h = OFF * 2;
	SDL_RenderFillRect(renderer, &cell);
	cell.x = endx - x - 2 * OFF + 1;
	cell.y = starty + y;
	cell.w = OFF * 2;
	cell.h = OFF * 2;
	SDL_RenderFillRect(renderer, &cell);
	x = x + 2 * OFF; y = y + 2 * OFF;
    }
}
