#include <stdio.h>
#include <SDL2/SDL.h>

int WINDOW_WIDTH = 1120;
int WINDOW_HEIGHT = 630;

#define WINDOW_RES 5

#define POS_X 210
#define POS_Y 50

SDL_Window* window;
SDL_Renderer* renderer;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int SCREEN_RES;

int** SCREEN;

void init_sdl()
{
    window = NULL; // window
    renderer = NULL; // renderer
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // initialize
    {
	printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	exit(1);
    }
    //Create window
    // SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    window = SDL_CreateWindow("plane", POS_X, POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    if( window == NULL )
    {
	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	exit(1);
    }
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void end_sdl()
{
    SDL_DestroyWindow( window ); // destroy window
    SDL_Quit(); // quit SDL subsystems
}

void graphics_init()
{
    init_sdl();

    SCREEN_RES = 5;
    SCREEN_WIDTH = WINDOW_WIDTH / SCREEN_RES;
    SCREEN_HEIGHT = WINDOW_HEIGHT / SCREEN_RES;

    SCREEN = (int**)malloc(SCREEN_HEIGHT * sizeof(int*));
    for(int i=0; i<SCREEN_HEIGHT; i++)
	SCREEN[i] = (int*)malloc(SCREEN_WIDTH * sizeof(int));

    for(int i=0; i<SCREEN_HEIGHT; i++)
	for(int j=0; j<SCREEN_WIDTH; j++)
	    SCREEN[i][j] = 0x0000ff; // blue screen
}

void graphics_end()
{
    for(int i=0; i<SCREEN_HEIGHT; i++)
	free(SCREEN[i]);
    free(SCREEN);
    end_sdl();
}

void graphics_reset(int color)
{
    int r, g, b; int tmp = color;
    b = tmp & 0xff; tmp = tmp >> 8;
    g = tmp & 0xff; tmp = tmp >> 8;
    r = tmp & 0xff;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    for(int i=0; i<SCREEN_HEIGHT; i++)
	for(int j=0; j<SCREEN_WIDTH; j++)
	    SCREEN[i][j] = color;
}

int curr_color;
void graphics_set_color(int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    curr_color = r & 0xff; curr_color = curr_color << 8;
    curr_color = curr_color | (g & 0xff); curr_color = curr_color << 8;
    curr_color = curr_color | (b & 0xff);
}

void graphics_update()
{
    SDL_RenderPresent(renderer);
}

void graphics_draw_rect(int x, int y, int w, int h)
{
    SDL_Rect cell;
    cell.x = x;
    cell.y = y;
    cell.w = w;
    cell.h = h;
    SDL_RenderFillRect(renderer, &cell);
}

int inside_screen(int row, int col)
{
    if(row < 0) return 0;
    if(row >= SCREEN_HEIGHT) return 0;
    if(col < 0) return 0;
    if(col >= SCREEN_WIDTH) return 0;
    return 1;
}

void screen_to_rowcol_coord(int screen_x, int screen_y, int* row, int* col)
{
    *col = screen_x + SCREEN_WIDTH / 2;
    *row = SCREEN_HEIGHT / 2 - screen_y;
}

// screen coord
void graphics_fill_box(int screen_x, int screen_y)
{
    int row, col;
    screen_to_rowcol_coord(screen_x, screen_y, &row, &col);

    if(!inside_screen(row, col)) return;

    SDL_Rect cell;
    cell.x = col * SCREEN_RES;
    cell.y = row * SCREEN_RES;
    cell.w = SCREEN_RES;
    cell.h = SCREEN_RES;
    SDL_RenderFillRect(renderer, &cell);

    SCREEN[row][col] = curr_color;
}
// row-col coord
void graphics_fill_cell(int row, int col) 
{
    if(!inside_screen(row, col)) return;
    SDL_Rect cell;
    cell.x = col * SCREEN_RES;
    cell.y = row * SCREEN_RES;
    cell.w = SCREEN_RES;
    cell.h = SCREEN_RES;
    SDL_RenderFillRect(renderer, &cell);
    SCREEN[row][col] = curr_color;
}

void recursive_fill(int row, int col, int old_color)
{
    graphics_fill_cell(row, col);
    int row1, col1;

    row1 = row - 1; col1 = col;
    if(inside_screen(row1, col1) && SCREEN[row1][col1] == old_color)
	recursive_fill(row1, col1, old_color);

    row1 = row + 1; col1 = col;
    if(inside_screen(row1, col1) && SCREEN[row1][col1] == old_color)
	recursive_fill(row1, col1, old_color);

    row1 = row; col1 = col - 1;
    if(inside_screen(row1, col1) && SCREEN[row1][col1] == old_color)
	recursive_fill(row1, col1, old_color);

    row1 = row; col1 = col + 1;
    if(inside_screen(row1, col1) && SCREEN[row1][col1] == old_color)
	recursive_fill(row1, col1, old_color);
}

void graphics_flood_fill(int screen_x, int screen_y)
{
    int row, col; screen_to_rowcol_coord(screen_x, screen_y, &row, &col);
    int old_color = SCREEN[row][col];

    // printf("flood_fill(): (%d, %d) --> (%d, %d)  old_color = 0x%x\n", screen_x, screen_y, row, col, old_color);

    if(old_color == curr_color) return;

    recursive_fill(row, col, old_color);
}

void graphics_midpoint_circle(int centre_x, int centre_y, int radius)
{
    // decision parameter D_i+1
    // ------------------------
    // y_i+1 = y_i + 1
    // x_i+1 = x_i - 1,    if D > 0
    // x_i+1 = x_i         otherwise
    // D_i+1 = 2[(x_i^2 + y_i^2 - r^2) + (2y_i + 1)] + (1 - 2x_i)
    //
    // algorithm:
    // ----------
    // x_0 = r, y_0 = 0
    // D_1 = 3 - 2r
    // D_i+2 = D_i+1 + 2[5 - 2x_i + 2y_i],  if D_i+1 > 0
    // D_i+2 = D_i+1 + 2[3 + 2y_i]          otherwise

    int D = 3 - 2*radius;
    int x = radius, y = 0;
    while(x >= y)
    {
	//     \ | /
	//      \|/
	//   ----+-----
	//      /|\ 
	//     / | \

	int tmp_x, tmp_y;

	// octant 1
	graphics_fill_box(centre_x + x, centre_y + y);

	// octant 2
	tmp_x = y; tmp_y = x; // reflect xy
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 3
	tmp_x = y; tmp_y = x; // reflect xy
	tmp_x = - tmp_x; // reflect y-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 4
	tmp_x = x; tmp_y = y; // kichu na
	tmp_x = - tmp_x; // reflect y-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 5
	tmp_x = x; tmp_y = y; // kichu na
	tmp_x = - tmp_x; // reflect y-axis
	tmp_y = - tmp_y; // reflect x-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 6
	tmp_x = y; tmp_y = x; // reflect xy
	tmp_x = - tmp_x; // reflect y-axis
	tmp_y = - tmp_y; // reflect x-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 7
	tmp_x = y; tmp_y = x; // reflect xy
	tmp_y = - tmp_y; // reflect x-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// octant 8
	tmp_x = x; tmp_y = y; // kichu na
	tmp_y = - tmp_y; // reflect x-axis
	graphics_fill_box(centre_x + tmp_x, centre_y + tmp_y);

	// update:
	// x,y _i -> x,y _i+1 (based on D_i+1)
	// D_i+1 -> D_i+2 (based on D_i+1)
	if(D > 0)
	{
	    D = D + 2*(5 - 2*x + 2*y);
	    x = x - 1;
	}
	else
	{
	    D = D + 2*(3 + 2*y);
	}
	y = y + 1;
    }
}
