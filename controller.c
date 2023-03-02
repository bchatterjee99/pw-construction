#include <SDL2/SDL.h>
#include <unistd.h>

#include "world.h"


int STOP;

void handle_key(SDL_Keycode sym)
{
    int speed = 20 + (- camera_z) * 20;
    switch(sym)
    {
    case SDLK_h: // LEFT
	camera_x -= speed;
	break;
    case SDLK_l: // RIGHT
	camera_x += speed;
	break;
    case SDLK_k: // UP
	camera_y += speed;
	break;
    case SDLK_j: // DOWN
	camera_y -= speed;
	break;
    case SDLK_z: // ZOOM OUT
	if(camera_z >= -70)
	camera_z -= 10;
	break;
    case SDLK_x: // ZOOM IN
	if(camera_z <= -10)
	    camera_z += 10;
	break;
    default:
	STOP = 1;
	break;
    }
}

void control()
{
    STOP = 0;
    while(1)
    {
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
	    switch(event.type)
	    {
	    case SDL_KEYDOWN:
		break;
	    case SDL_KEYUP:
		handle_key(event.key.keysym.sym);
		break;
	    }
	    if(STOP) break;
	}
	if(STOP) break;
	world_draw_world();
	usleep(10000);
    }
}
