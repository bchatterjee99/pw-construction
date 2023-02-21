#include <SDL2/SDL.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void draw_init();

void draw_reset();

void draw_set_color(int r, int g, int b);

void draw_update();

void draw_end();


