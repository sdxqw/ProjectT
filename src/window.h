#pragma once
#include <SDL_render.h>
#include <SDL_video.h>

#include "player.h"
#include "world.h"

#define LOG_INFO(...) fprintf(stdout, __VA_ARGS__)
#define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)

typedef struct {
    char *title;
    int width;
    int height;
} WindowInfo;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    WindowInfo *info;
    World *world;
} Window;

Window *create_window(WindowInfo *info);

void render_window(const Window *window);

void update_window(const Window *window);

void destroy_window(Window *window);
