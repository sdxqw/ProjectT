#pragma once
#include "camera.h"
#include "player.h"

typedef struct {
    int *x;
    int *y;
    int *tile_index;
    int count;
} WorldInfo;

typedef struct {
    Player *player;
    SpriteSheet *sprite_sheet_grass;
    TileSet *tile_grass;
    WorldInfo *world_info;
    Camera *camera;
} World;

World *create_world(SDL_Renderer *renderer);

WorldInfo *create_world_info(const char *path_to_file);

void render_world(SDL_Renderer *renderer, const World *world, int screen_width, int screen_height);

void update_world(float delta_time, const World *world, int screen_width, int screen_height);

void destroy_world(World *world);
