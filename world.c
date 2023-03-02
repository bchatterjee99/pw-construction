#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "keyboard.h"

struct object{
    int global_x;
    int global_y;
    int global_z;
    int radius;
    int color;
};

struct object* object_list;
int obj_idx;
int max_objects;

int camera_x, camera_y, camera_z;


void world_init(int num_objects)
{
    object_list = (struct object *)malloc(num_objects * sizeof(struct object));
    max_objects = num_objects;
    obj_idx = 0;
}

void world_draw_object(int idx)
{
    // global coordinates
    int global_x = object_list[idx].global_x;
    int global_y = object_list[idx].global_y;
    int global_z = object_list[idx].global_z;
    int radius = object_list[idx].radius;

    // coordinates with camera as origin
    int camera_coord_x = global_x - camera_x;
    int camera_coord_y = global_y - camera_y;
    int camera_coord_z = global_z - camera_z;

    // projection on z=1 plane in camera-coordinates
    int screen_x = camera_coord_x / camera_coord_z;
    int screen_y = camera_coord_y / camera_coord_z;
    radius = radius / camera_coord_z;

    graphics_midpoint_circle(screen_x, screen_y, radius);
}

void world_draw_world()
{
    graphics_reset(0x000020);
    graphics_set_color(255, 0, 0);
    for(int i=0; i<obj_idx; i++)
	world_draw_object(i);
    graphics_update();
}

void world_add_object(int pos_x, int pos_y, int pos_z, int radius)
{
    if(obj_idx >= max_objects)
    {
	printf("num objects > max num ojects\n\n");
	return;
    }

    object_list[obj_idx].global_x = pos_x;
    object_list[obj_idx].global_y = pos_y;
    object_list[obj_idx].global_z = pos_z;
    object_list[obj_idx].radius = radius;
    obj_idx++;
}

#define SPEED 10
void worldanimate()
{
    camera_x = 0;
    camera_y = 0;
    camera_z = 0;

    int counter = (1 << 20);
    while(counter--)
    {
	world_draw_world();
	camera_x += SPEED;
	camera_z -= 1;
	if(key_press()) break;
	usleep(500000);
    }
}

void world_test()
{
    world_init(5);
    world_add_object(10, 10, 10, 100);
    worldanimate();
}
