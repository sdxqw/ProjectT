#pragma once
#include <SDL_render.h>

typedef struct {
    char *name;
    SDL_Texture *texture;
    int sprite_width;
    int sprite_height;
} SpriteSheet;

typedef struct {
    SpriteSheet *sprite_sheet;
    int start_index;
    int end_index;
    int current_frame;
    float frame_timer;
    float frame_duration;
} Animation;

typedef struct {
    SpriteSheet *sprite_sheet;
    int tile_index;
} Tile;

typedef struct {
    SpriteSheet *sprite_sheet;
    Tile **tiles;
    int tile_count;
} TileSet;

SpriteSheet *load_sprite_sheet(SDL_Renderer *renderer, char *path_to_file, int sprite_width,
                               int sprite_height, int rows, int columns);

Animation *create_animation(SpriteSheet *sprite_sheet, int start_index, int end_index, float frame_duration);

Tile *create_tile(SpriteSheet *sprite_sheet, int tile_index);

TileSet *create_tile_set(SpriteSheet *sprite_sheet, int *tile_indices, int tile_count);

void update_animation(Animation *animation, float delta_time);

void render_animation(const Animation *animation, SDL_Renderer *renderer, int x, int y,
                      const float *scale_x, const float *scale_y, int flipped);

void render_tile(const Tile *tile, SDL_Renderer *renderer, int x, int y, const float *scale_x, const float *scale_y,
                 int flipped);

void render_tile_set(const TileSet *tile_set, SDL_Renderer *renderer, int x, int y, const float *scale_x,
                     const float *scale_y, int flipped);

void destroy_sprite_sheet(SpriteSheet *sprite_sheet);

void destroy_animation(Animation *animation);

void destroy_tile(Tile *tile);

void destroy_tile_set(TileSet *tile_set);

