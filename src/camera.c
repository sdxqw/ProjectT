#include "camera.h"

#include <stdlib.h>

#include "window.h"


Camera *create_camera(const int x, const int y, const float zoom) {
    Camera *camera = malloc(sizeof(Camera));
    if (camera) {
        camera->x = x;
        camera->y = y;
        camera->zoom = zoom;
    }
    return camera;
}

void update_camera(Camera *camera, const int x, const int y, const float zoom) {
    if (camera) {
        camera->zoom += zoom;
        camera->x += x;
        camera->y += y;
    }
}

void look_at(Camera *camera, const int x, const int y, const int screen_width, const int screen_height) {
    if (camera) {
        camera->x = x - camera->x / camera->zoom / 2;
        camera->y = y - camera->y / camera->zoom / 2;
    }
}

SDL_Rect get_camera_viewport(const Camera *camera, const int screen_width, const int screen_height) {
    SDL_Rect viewport;
    viewport.x = camera->x;
    viewport.y = camera->y;
    if (camera->zoom != 0) {
        viewport.w = screen_width / (int) camera->zoom;
        viewport.h = screen_height / (int) camera->zoom;
    } else {
        viewport.w = screen_width;
        viewport.h = screen_height;
    }
    return viewport;
}

