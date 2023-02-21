#include <SDL2/SDL.h>


// returns 1 on key press
int key_press()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	    return 1;
    }
    return 0;
}
