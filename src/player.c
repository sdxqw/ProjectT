#include "player.h"

#include <SDL_keyboard.h>
#include <stdio.h>
#include <stdlib.h>

#include "window.h"

Player *create_player(SDL_Renderer *renderer) {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        LOG_ERROR("Cannot allocate memory for player\n");
        return NULL;
    }

    LOG_INFO("Created the player\n");

    // Load sprite sheet
    player->sprite_sheet = load_sprite_sheet(renderer, "assets/sprite_sheet_player.png", 192, 192, 4, 4);
    if (!player->sprite_sheet) {
        LOG_ERROR("Failed to load player sprite sheet\n");
        destroy_player(player);
        return NULL;
    }

    LOG_INFO("Created the player sprite sheet\n");

    const int animations[8][2] = {
        {0, 1}, // Idle
        {2, 3}, // Walking right
        {4, 5}, // Walking left
        {6, 7}, // ??
        {8, 9},
        {10, 11},
        {12, 13},
        {14, 15}
    };

    for (int i = 0; i < 8; i++) {
        player->animation[i] = create_animation(player->sprite_sheet, animations[i][0], animations[i][1], 0.15f);
        if (!player->animation[i]) {
            LOG_ERROR("Failed to create player animation\n");
            destroy_player(player);
            return NULL;
        }
    }

    LOG_INFO("Create the player animations\n");

    player->x = 0;
    player->y = 0;
    player->health = 100;
    player->mana = 100;
    player->speed = 5;
    player->current_animation = 0;
    player->last_animation = 0;
    return player;
}


void render_player(const Player *player, SDL_Renderer *renderer, const Camera *camera) {
    const int render_x = (player->x - camera->x) * camera->zoom;
    const int render_y = (player->y - camera->y) * camera->zoom;
    render_animation(player->animation[player->current_animation], renderer, render_x, render_y,
                     &camera->zoom, &camera->zoom, 0);
}

void update_player(Player *player, const float delta_time) {
    // Example: Change animation based on position
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_RIGHT]) {
        player->x += player->speed;
        player->current_animation = 7; // Walking right
        player->last_animation = 6;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        player->x -= player->speed;
        player->current_animation = 5; // Walking left
        player->last_animation = 4;
    }
    if (state[SDL_SCANCODE_UP]) {
        player->y -= player->speed;
        player->current_animation = 3; // Walking up
        player->last_animation = 2;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        player->y += player->speed;
        player->current_animation = 1; // Walking down
        player->last_animation = 0;
    }

    if (!state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_UP] && !state[
            SDL_SCANCODE_DOWN]) {
        player->current_animation = player->last_animation;
    }
    update_animation(player->animation[player->current_animation], delta_time);
}

void destroy_player(Player *player) {
    if (player) {
        if (player->sprite_sheet) {
            destroy_sprite_sheet(player->sprite_sheet);
            LOG_INFO("Destroyed the sprite sheet FROM PLAYER\n");
        }
        for (int i = 0; i < 4; i++) {
            if (player->animation[i]) {
                destroy_animation(player->animation[i]);
                LOG_INFO("Destroyed the animation FROM PLAYER\n");
            }
        }
        LOG_INFO("Destroyed the player\n");
        free(player);
    }
}
