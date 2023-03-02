
struct object{
    int global_x;
    int global_y;
    int global_z;
    int radius;
    int color;
};

extern int camera_x;
extern int camera_y;
extern int camera_z;


void world_init(int num_objects);

void world_draw_object(int idx);

void world_draw_world();

void world_add_object(int pos_x, int pos_y, int pos_z, int radius);

void worldanimate();

void world_test();
