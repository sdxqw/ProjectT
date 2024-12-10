#include "world.h"

#include <stdio.h>
#include <stdlib.h>

#include "window.h"


WorldInfo *create_world_info(const char *path_to_file) {
    WorldInfo *world_info = malloc(sizeof(WorldInfo));
    if (!world_info) {
        LOG_ERROR("Failed to allocate memory for world info\n");
        return NULL;
    }

    FILE *file = fopen(path_to_file, "r");
    if (!file) {
        LOG_ERROR("Failed to open file: %s\n", path_to_file);
        free(world_info);
        return NULL;
    }

    world_info->x = NULL;
    world_info->y = NULL;
    world_info->tile_index = NULL;
    world_info->count = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        world_info->x = realloc(world_info->x, (world_info->count + 1) * sizeof(int));
        world_info->y = realloc(world_info->y, (world_info->count + 1) * sizeof(int));
        world_info->tile_index = realloc(world_info->tile_index, (world_info->count + 1) * sizeof(int));

        if (!world_info->x || !world_info->y || !world_info->tile_index) {
            LOG_ERROR("Failed to allocate memory for world info arrays\n");
            free(world_info->x);
            free(world_info->y);
            free(world_info->tile_index);
            free(world_info);
            fclose(file);
            return NULL;
        }

        char *endptr = line;
        world_info->x[world_info->count] = strtol(endptr + 1, &endptr, 10); // Skip '['
        world_info->y[world_info->count] = strtol(endptr + 1, &endptr, 10); // Skip ','
        world_info->tile_index[world_info->count] = strtol(endptr + 1, &endptr, 10); // Skip ','
        world_info->count++;
    }

    fclose(file);
    return world_info;
}

World *create_world(SDL_Renderer *renderer) {
    World *world = malloc(sizeof(World));
    if (!world) {
        LOG_ERROR("Cannot allocate memory for world\n");
        return NULL;
    }

    world->player = create_player(renderer);
    world->sprite_sheet_grass = load_sprite_sheet(renderer, "assets/sprite_sheet_grass.png", 176, 112, 11, 7);
    int tile_indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    world->tile_grass = create_tile_set(world->sprite_sheet_grass, tile_indices, 11 * 7);
    world->world_info = create_world_info("assets/world.txt");
    return world;
}


void render_world(SDL_Renderer *renderer, const World *world) {
    for (int i = 0; i < world->world_info->count; i++) {
        const int x = world->world_info->x[i] * 16 * 4;
        const int y = world->world_info->y[i] * 16 * 4;
        const int tile_index = world->world_info->tile_index[i];
        float new_size = 4.f;
        render_tile(world->tile_grass->tiles[tile_index], renderer, x, y, &new_size, &new_size, 0);
    }
    render_player(world->player, renderer);
}

void update_world(const float delta_time, const World *world) {
    update_player(world->player, delta_time);
}

void destroy_world(World *world) {
    if (world) {
        destroy_player(world->player);
        destroy_tile_set(world->tile_grass);
        destroy_sprite_sheet(world->sprite_sheet_grass);
        free(world);
    }
}