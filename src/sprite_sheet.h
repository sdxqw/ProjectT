#pragma once
#include <SDL_render.h>


typedef struct {
    char* name;
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


SpriteSheet *load_sprite_sheet(SDL_Renderer *renderer,  char *path_to_file, int sprite_width,
                               int sprite_height, int rows, int columns);

Animation *create_animation(SpriteSheet *sprite_sheet, int start_index, int end_index, float frame_duration);

void render_sprite_sheet(const SpriteSheet *sprite_sheet, SDL_Renderer *renderer, int x, int y,
                         int sprite_index, const float *scale_x, const float *scale_y, int flipped);

void update_animation(Animation *animation, float delta_time);

void render_sprite_sheet_as_animation(const Animation *animation, SDL_Renderer *renderer, int x, int y,
                                      const float *scale_x, const float *scale_y, int flipped);

void update_size(const SpriteSheet *sprite_sheet, int sprite_width, int sprite_height);

void destroy_sprite_sheet(SpriteSheet *sprite_sheet);

void destroy_animation(Animation *animation);

