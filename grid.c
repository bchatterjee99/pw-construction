#include <math.h>
#include "draw.h"

#include "grid.h"

int cell_size;
int num_row; // number of rows
int num_col; // number of columns

void draw_grid(int num_cell)
{
    int tmp = SCREEN_WIDTH * SCREEN_HEIGHT / num_cell;
    cell_size = sqrt(tmp);
    num_row = SCREEN_HEIGHT / cell_size;
    num_col = SCREEN_WIDTH / cell_size;
    printf("cell-size = %d  num-row = %d  num-col= %d\n\n", cell_size, num_row, num_col);

    SDL_Rect cell;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i=0; i<num_row; i++)
    {
	for(int j=0; j<num_col; j++)
	{
	    cell.x = j * cell_size + OFF;
	    cell.y = i * cell_size + OFF;
	    cell.w = cell_size - 2 * OFF;
	    cell.h = cell_size - 2 * OFF;
	    SDL_RenderFillRect(renderer, &cell);
	}
    }
    SDL_RenderPresent(renderer);
}
