#pragma once
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
} World;

World *create_world(SDL_Renderer *renderer);

WorldInfo *create_world_info(const char *path_to_file);

void render_world(SDL_Renderer *renderer, const World *world);

void update_world(float delta_time, const World *world);

void destroy_world(World *world);