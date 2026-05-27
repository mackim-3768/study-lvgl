/**
 * watch_app — Galaxy Watch sized LVGL stopwatch demo.
 *
 * Target: 466 x 466 SDL window emulating the watch display.
 */

#include "lvgl.h"
#include "src/drivers/sdl/lv_sdl_window.h"
#include "src/drivers/sdl/lv_sdl_mouse.h"
#include "src/drivers/sdl/lv_sdl_mousewheel.h"
#include "src/drivers/sdl/lv_sdl_keyboard.h"

#include "screens/stopwatch_ui.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#define WATCH_HOR_RES 466
#define WATCH_VER_RES 466

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    lv_init();

    lv_display_t *disp = lv_sdl_window_create(WATCH_HOR_RES, WATCH_VER_RES);
    if (disp == NULL) {
        fprintf(stderr, "failed to create SDL display\n");
        return 1;
    }
    lv_sdl_window_set_title(disp, "Galaxy Watch — Stopwatch");

    lv_indev_t *mouse = lv_sdl_mouse_create();
    (void)mouse;

    stopwatch_ui_create();

    while (1) {
        uint32_t idle_ms = lv_timer_handler();
        if (idle_ms > 16) idle_ms = 16;
        SDL_Delay(idle_ms);
    }

    return 0;
}
