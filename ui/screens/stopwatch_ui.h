#ifndef STOPWATCH_UI_H
#define STOPWATCH_UI_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Build the stopwatch screen on the active display and load it.
 * Must be called after lv_init() and the display has been created.
 */
void stopwatch_ui_create(void);

#ifdef __cplusplus
}
#endif

#endif /* STOPWATCH_UI_H */
