#pragma once
#include "sprite_sheet.h"

typedef struct {
    int x, y;
    int health;
    int mana;
    int speed;
    int current_animation;
    int last_animation;
    SpriteSheet *sprite_sheet;
    Animation *animation[8];
} Player;

Player *create_player(SDL_Renderer *renderer);

void render_player(const Player *player, SDL_Renderer *renderer);

void update_player(Player *player, float delta_time);

void destroy_player(Player *player);
