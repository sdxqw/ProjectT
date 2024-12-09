#include "window.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <wchar.h>

Window *create_window(WindowInfo *info) {
    Window *window = malloc(sizeof(Window));
    if (!window) {
        LOG_ERROR("Failed to allocate memory for window\n");
        return NULL;
    }

    LOG_INFO("Created the window\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_ERROR("SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }

    window->window = SDL_CreateWindow(info->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      info->width, info->height,
                                      SDL_WINDOW_SHOWN);
    if (window->window == NULL) {
        LOG_ERROR("SDL_CreateWindow Error: %s\n", SDL_GetError());
        free(window);
        SDL_Quit();
        return NULL;
    }
    LOG_INFO("Created the window SDL\n");

    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window->renderer == NULL) {
        LOG_ERROR("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroy_window(window);
        return NULL;
    }
    LOG_INFO("Created the renderer\n");

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        LOG_ERROR("IMG_Init Error: %s\n", IMG_GetError());
        destroy_window(window);
        return NULL;
    }
    LOG_INFO("Initialized SDL_image\n");

    window->player = create_player(window->renderer);
    if (!window->player) {
        LOG_ERROR("Failed to create player\n");
        destroy_window(window);
        return NULL;
    }
    LOG_INFO("Created the player\n");

    window->info = info;

    return window;
}


void render_window(const Window *window) {
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 0);
    SDL_RenderClear(window->renderer);
    render_player(window->player, window->renderer);
    SDL_RenderPresent(window->renderer);
}

void update_window(const Window *window) {
    int running = 1;
    SDL_Event e;
    while (running) {
        const float delta_time = 0.016f;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        update_player(window->player, delta_time);
        render_window(window);
        SDL_Delay(16);
    }
}


void destroy_window(Window *window) {
    if (window) {
        if (window->window) {
            SDL_DestroyWindow(window->window);
            LOG_INFO("Destroyed the window SDL FROM WINDOW\n");
        }
        if (window->renderer) {
            SDL_DestroyRenderer(window->renderer);
            LOG_INFO("Destroyed the renderer FROM WINDOW\n");
        }
        if (window->player) {
            destroy_player(window->player);
            LOG_INFO("Destroyed the player FROM WINDOW\n");
        }
        free(window);
        LOG_INFO("Destroyed the window\n");
    }
    SDL_Quit();
}
