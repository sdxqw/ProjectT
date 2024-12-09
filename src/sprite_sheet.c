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


void render_sprite_sheet(const SpriteSheet *sprite_sheet, SDL_Renderer *renderer, const int x, const int y,
                         const int sprite_index, const float *scale_x, const float *scale_y, const int flipped) {
    int texture_width, texture_height;
    SDL_QueryTexture(sprite_sheet->texture, NULL, NULL, &texture_width, &texture_height);

    const int sprites_per_row = texture_width / sprite_sheet->sprite_width;

    // Calculate the position of the sprite on the sprite sheet
    const SDL_Rect src_rect = {
        sprite_index % sprites_per_row * sprite_sheet->sprite_width,
        sprite_index / sprites_per_row * sprite_sheet->sprite_height,
        sprite_sheet->sprite_width,
        sprite_sheet->sprite_height,
    };

    // Determine scaling factors (use default if NULL or scale factors are 1.0)
    const float scale_x_val = scale_x ? *scale_x : 1.0f;
    const float scale_y_val = scale_y ? *scale_y : 1.0f;

    // Calculate the destination rectangle, optionally applying scaling
    const SDL_Rect dst_rect = {
        x,
        y,
        (sprite_sheet->sprite_width * (int) scale_x_val),
        (sprite_sheet->sprite_height * (int) scale_y_val),
    };

    // Determine the flip flag
    SDL_RendererFlip flip_flag = SDL_FLIP_NONE;
    if (flipped == 1) {
        flip_flag = SDL_FLIP_HORIZONTAL; // Flip horizontally
    } else if (flipped == 2) {
        flip_flag = SDL_FLIP_VERTICAL; // Flip vertically
    }

    // Render the sprite with optional flipping
    SDL_RenderCopyEx(renderer, sprite_sheet->texture, &src_rect, &dst_rect, 0, NULL, flip_flag);
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


void render_sprite_sheet_as_animation(const Animation *animation, SDL_Renderer *renderer, const int x, const int y,
                                      const float *scale_x, const float *scale_y, const int flipped) {
    render_sprite_sheet(animation->sprite_sheet, renderer, x, y, animation->current_frame, scale_x, scale_y, flipped);
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
