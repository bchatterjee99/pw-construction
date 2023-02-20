#include <stdio.h>
#include "draw.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Window* window;
SDL_Renderer* renderer;

void draw_init()
{
    window = NULL; // window
    renderer = NULL; // renderer
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // initialize
    {
	printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	exit(1);
    }
    //Create window
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
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

void draw_end()
{
    SDL_DestroyWindow( window ); // destroy window
    SDL_Quit(); // quit SDL subsystems
}

int draw_test_1( int argc, char* args[] )
{
    draw_init();
    char ch = fgetc(stdin);
    draw_end();
    return 0;
}


