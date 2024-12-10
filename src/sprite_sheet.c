#include "sprite_sheet.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>

#include "window.h"

SpriteSheet *load_sprite_sheet(SDL_Renderer *renderer, char *path_to_file, const int sprite_width,
                               const int sprite_height, const int rows, const int columns) {
    SpriteSheet *sprite_sheet = malloc(sizeof(SpriteSheet));
    if (!sprite_sheet) {
        LOG_ERROR("Failed to allocate memory for sprite sheet\n");
        return NULL;
    }
    LOG_INFO("Created the sprite sheet for the path: %s\n", path_to_file);

    SDL_Surface *surface = IMG_Load(path_to_file);
    if (!surface) {
        LOG_ERROR("Failed to load image: %s\n", IMG_GetError());
        free(sprite_sheet);
        return NULL;
    }

    sprite_sheet->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!sprite_sheet->texture) {
        LOG_ERROR("Failed to create a texture from surface: %s\n", SDL_GetError());
        free(sprite_sheet);
        return NULL;
    }
    LOG_INFO("Created the surface for the sprite sheet %s\n", path_to_file);

    sprite_sheet->sprite_width = sprite_width / rows;
    sprite_sheet->sprite_height = sprite_height / columns;

    sprite_sheet->name = path_to_file;

    return sprite_sheet;
}


Animation *create_animation(SpriteSheet *sprite_sheet, const int start_index, const int end_index,
                            const float frame_duration) {
    Animation *animation = malloc(sizeof(Animation));
    if (!animation) {
        LOG_ERROR("Failed to allocate memory for animation\n");
        return NULL;
    }

    LOG_INFO("Created the animation for: %s\n", sprite_sheet->name);

    animation->sprite_sheet = sprite_sheet;
    animation->start_index = start_index;
    animation->end_index = end_index;
    animation->current_frame = start_index;
    animation->frame_timer = 0.0f;
    animation->frame_duration = frame_duration;

    return animation;
}

Tile *create_tile(SpriteSheet *sprite_sheet, const int tile_index) {
    Tile *tile = malloc(sizeof(Tile));
    if (!tile) {
        LOG_ERROR("Failed to allocate memory for tile\n");
        return NULL;
    }
    tile->sprite_sheet = sprite_sheet;
    tile->tile_index = tile_index;
    return tile;
}

TileSet *create_tile_set(SpriteSheet *sprite_sheet, int *tile_indices, int tile_count) {
    TileSet *tile_set = malloc(sizeof(TileSet));
    if (!tile_set) {
        LOG_ERROR("Failed to allocate memory for tile set\n");
        return NULL;
    }
    tile_set->sprite_sheet = sprite_sheet;
    tile_set->tiles = malloc(tile_count * sizeof(Tile *));
    if (!tile_set->tiles) {
        LOG_ERROR("Failed to allocate memory for tiles\n");
        free(tile_set);
        return NULL;
    }
    for (int i = 0; i < tile_count; i++) {
        tile_set->tiles[i] = create_tile(sprite_sheet, tile_indices[i]);
    }
    tile_set->tile_count = tile_count;
    return tile_set;
}

void update_animation(Animation *animation, const float delta_time) {
    animation->frame_timer += delta_time;

    // Move to the next frame if the timer exceeds the frame duration
    if (animation->frame_timer >= animation->frame_duration) {
        animation->frame_timer -= animation->frame_duration;
        animation->current_frame++;

        // Loop back to the start index if the animation reaches the end
        if (animation->current_frame > animation->end_index) {
            animation->current_frame = animation->start_index;
        }
    }
}


void render_animation(const Animation *animation, SDL_Renderer *renderer, const int x, const int y,
                      const float *scale_x, const float *scale_y, const int flipped) {
    int texture_width, texture_height;
    SDL_QueryTexture(animation->sprite_sheet->texture, NULL, NULL, &texture_width, &texture_height);

    const int sprites_per_row = texture_width / animation->sprite_sheet->sprite_width;

    const SDL_Rect src_rect = {
        animation->current_frame % sprites_per_row * animation->sprite_sheet->sprite_width,
        animation->current_frame / sprites_per_row * animation->sprite_sheet->sprite_height,
        animation->sprite_sheet->sprite_width,
        animation->sprite_sheet->sprite_height,
    };

    const float scale_x_val = scale_x ? *scale_x : 1.0f;
    const float scale_y_val = scale_y ? *scale_y : 1.0f;

    const SDL_Rect dst_rect = {
        x,
        y,
        (animation->sprite_sheet->sprite_width * (int) scale_x_val),
        (animation->sprite_sheet->sprite_height * (int) scale_y_val),
    };

    SDL_RendererFlip flip_flag = SDL_FLIP_NONE;
    if (flipped == 1) {
        flip_flag = SDL_FLIP_HORIZONTAL;
    } else if (flipped == 2) {
        flip_flag = SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyEx(renderer, animation->sprite_sheet->texture, &src_rect, &dst_rect, 0, NULL, flip_flag);
}

void render_tile(const Tile *tile, SDL_Renderer *renderer, int x, int y, const float *scale_x, const float *scale_y,
                 int flipped) {
    int texture_width, texture_height;
    SDL_QueryTexture(tile->sprite_sheet->texture, NULL, NULL, &texture_width, &texture_height);

    const int sprites_per_row = texture_width / tile->sprite_sheet->sprite_width;

    const SDL_Rect src_rect = {
        tile->tile_index % sprites_per_row * tile->sprite_sheet->sprite_width,
        tile->tile_index / sprites_per_row * tile->sprite_sheet->sprite_height,
        tile->sprite_sheet->sprite_width,
        tile->sprite_sheet->sprite_height,
    };

    const float scale_x_val = scale_x ? *scale_x : 1.0f;
    const float scale_y_val = scale_y ? *scale_y : 1.0f;

    const SDL_Rect dst_rect = {
        x,
        y,
        (tile->sprite_sheet->sprite_width * (int) scale_x_val),
        (tile->sprite_sheet->sprite_height * (int) scale_y_val),
    };

    SDL_RendererFlip flip_flag = SDL_FLIP_NONE;
    if (flipped == 1) {
        flip_flag = SDL_FLIP_HORIZONTAL;
    } else if (flipped == 2) {
        flip_flag = SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyEx(renderer, tile->sprite_sheet->texture, &src_rect, &dst_rect, 0, NULL, flip_flag);
}

void render_tile_set(const TileSet *tile_set, SDL_Renderer *renderer, const int x, const int y, const float *scale_x,
                     const float *scale_y, const int flipped) {
    for (int i = 0; i < tile_set->tile_count; i++) {
        int tile_x = x + tile_set->tiles[i]->tile_index % 11 * tile_set->sprite_sheet->sprite_width;
        int tile_y = y + tile_set->tiles[i]->tile_index / 11 * tile_set->sprite_sheet->sprite_height;
        render_tile(tile_set->tiles[i], renderer, tile_x, tile_y, scale_x, scale_y, flipped);
    }
}


void destroy_sprite_sheet(SpriteSheet *sprite_sheet) {
    if (sprite_sheet) {
        if (sprite_sheet->texture) {
            SDL_DestroyTexture(sprite_sheet->texture);
            LOG_INFO("Destroyed the texture of the sprite sheet\n");
        }
        LOG_INFO("Destroyed the sprite sheet\n");
        free(sprite_sheet);
    }
}

void destroy_animation(Animation *animation) {
    if (animation) {
        free(animation);
        LOG_INFO("Destroyed the animation\n");
    }
}


void destroy_tile(Tile *tile) {
    if (tile) {
        free(tile);
        LOG_INFO("Destroyed the tile\n");
    }
}

void destroy_tile_set(TileSet *tile_set) {
    if (tile_set) {
        for (int i = 0; i < tile_set->tile_count; i++) {
            destroy_tile(tile_set->tiles[i]);
        }
        free(tile_set->tiles);
        free(tile_set);
        LOG_INFO("Destroyed the tile set\n");
    }
}
