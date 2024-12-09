#include <stdio.h>
#include <SDL.h>
#include "window.h"


int main(int argc, char *argv[]) {
    WindowInfo window_info = {"ProjectT", 800, 600,};
    Window *window = create_window(&window_info);

    if (!window) return -1;

    update_window(window);
    destroy_window(window);
    return 0;
}
