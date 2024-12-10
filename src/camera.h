#pragma once
#include <SDL_rect.h>

typedef struct {
    int x, y;
    float zoom;
} Camera;

Camera *create_camera(int x, int y, float zoom);

void update_camera(Camera *camera, int x, int y, float zoom);

void look_at(Camera *camera, int x, int y, int screen_width, int screen_height);

SDL_Rect get_camera_viewport(const Camera *camera, int screen_width, int screen_height);
