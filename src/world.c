#include "world.h"

#include <SDL_keyboard.h>
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
        int *new_x = realloc(world_info->x, (world_info->count + 1) * sizeof(int));
        int *new_y = realloc(world_info->y, (world_info->count + 1) * sizeof(int));
        int *new_tile_index = realloc(world_info->tile_index, (world_info->count + 1) * sizeof(int));

        if (!new_x || !new_y || !new_tile_index) {
            LOG_ERROR("Failed to allocate memory for world info arrays\n");
            free(new_x);
            free(new_y);
            free(new_tile_index);
            free(world_info->x);
            free(world_info->y);
            free(world_info->tile_index);
            free(world_info);
            fclose(file);
            return NULL;
        }

        world_info->x = new_x;
        world_info->y = new_y;
        world_info->tile_index = new_tile_index;

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
    world->camera = create_camera(0, 0, 5.0f);
    return world;
}


void render_world(SDL_Renderer *renderer, const World *world, const int screen_width, const int screen_height) {
    const SDL_Rect viewport = get_camera_viewport(world->camera, screen_width, screen_height);

    for (int i = 0; i < world->world_info->count; i++) {
        const int x = (world->world_info->x[i] * 16 - viewport.x) * (int) world->camera->
                      zoom;
        const int y = (world->world_info->y[i] * 16 - viewport.y) * (int) world->camera->
                      zoom;
        render_tile(world->tile_grass->tiles[world->world_info->tile_index[i]], renderer, x, y, &world->camera->zoom,
                    &world->camera->zoom,
                    0);
    }
    render_player(world->player, renderer, world->camera); // Pass camera to render_player
}

void update_world(const float delta_time, const World *world, int screen_width, int screen_height) {
    update_player(world->player, delta_time);
    look_at(world->camera, world->player->x, world->player->y, screen_width, screen_height);
}

void destroy_world(World *world) {
    if (world) {
        destroy_player(world->player);
        destroy_tile_set(world->tile_grass);
        destroy_sprite_sheet(world->sprite_sheet_grass);
        free(world);
    }
}
