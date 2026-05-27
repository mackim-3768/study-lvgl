/**
 * @file lv_conf.h
 * LVGL 9.x configuration for the Galaxy Watch sized stopwatch demo.
 *
 * Target display: 466 x 466 circular.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
 *   COLOR / DISPLAY
 *====================*/
#define LV_COLOR_DEPTH               32

/*====================
 *   MEMORY
 *====================*/
#define LV_USE_STDLIB_MALLOC         LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING         LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF        LV_STDLIB_BUILTIN

#define LV_MEM_SIZE                  (256U * 1024U)
#define LV_MEM_POOL_INCLUDE          <stdlib.h>

/*====================
 *   HAL / TICK
 *====================*/
#define LV_DEF_REFR_PERIOD           16   /* ~60 FPS */
#define LV_DPI_DEF                   200

#define LV_TICK_CUSTOM               1
#define LV_TICK_CUSTOM_INCLUDE       "SDL2/SDL.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (SDL_GetTicks())

/*====================
 *   DRAW
 *====================*/
#define LV_USE_DRAW_SW               1
#define LV_DRAW_SW_COMPLEX           1

/*====================
 *   LOG
 *====================*/
#define LV_USE_LOG                   0

/*====================
 *   ASSERT
 *====================*/
#define LV_USE_ASSERT_NULL           1
#define LV_USE_ASSERT_MALLOC         1

/*====================
 *   FONTS
 *====================*/
#define LV_FONT_MONTSERRAT_14        1
#define LV_FONT_MONTSERRAT_20        1
#define LV_FONT_MONTSERRAT_28        1
#define LV_FONT_MONTSERRAT_48        1

#define LV_FONT_DEFAULT              &lv_font_montserrat_20

/*====================
 *   WIDGETS
 *====================*/
#define LV_USE_LABEL                 1
#define LV_LABEL_TEXT_SELECTION      1
#define LV_LABEL_LONG_TXT_HINT       1

#define LV_USE_BUTTON                1
#define LV_USE_BTNMATRIX             1
#define LV_USE_LIST                  1
#define LV_USE_OBJ                   1
#define LV_USE_ARC                   1
#define LV_USE_BAR                   1
#define LV_USE_SLIDER                1
#define LV_USE_SWITCH                1
#define LV_USE_TEXTAREA              1
#define LV_USE_CHECKBOX              1
#define LV_USE_LINE                  1
#define LV_USE_IMAGE                 1
#define LV_USE_ROLLER                1
#define LV_USE_DROPDOWN              1

/*====================
 *   THEMES
 *====================*/
#define LV_USE_THEME_DEFAULT         1
#define LV_THEME_DEFAULT_DARK        1
#define LV_THEME_DEFAULT_GROW        1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80

/*====================
 *   LAYOUTS
 *====================*/
#define LV_USE_FLEX                  1
#define LV_USE_GRID                  1

/*====================
 *   OS / DRIVERS
 *====================*/
#define LV_USE_OS                    LV_OS_NONE
#define LV_USE_SDL                   1
#define LV_SDL_INCLUDE_PATH          <SDL2/SDL.h>
#define LV_SDL_RENDER_MODE           LV_DISPLAY_RENDER_MODE_DIRECT
#define LV_SDL_BUF_COUNT             1
#define LV_SDL_FULLSCREEN            0
#define LV_SDL_DIRECT_EXIT           1
#define LV_SDL_MOUSEWHEEL_MODE       LV_SDL_MOUSEWHEEL_MODE_ENCODER

/*====================
 *   EXAMPLES / DEMOS
 *====================*/
#define LV_BUILD_EXAMPLES            0
#define LV_USE_DEMO_WIDGETS          0

#endif /* LV_CONF_H */
