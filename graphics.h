

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_RES;

void graphics_init();
void graphics_end();

void graphics_reset();

void graphics_set_color(int r, int g, int b);

void graphics_draw_rect(int x, int y, int w, int h);

void graphics_update();

void graphics_flood_fill(int screen_x, int screen_y);

void graphics_midpoint_circle(int centre_x, int centre_y, int radius);

void graphics_save(char* filename);

