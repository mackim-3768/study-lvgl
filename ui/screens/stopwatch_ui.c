#include "stopwatch_ui.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SCREEN_SIZE 466
#define RING_PAD    8

typedef enum {
    SW_STATE_IDLE = 0,
    SW_STATE_RUNNING,
    SW_STATE_PAUSED,
} sw_state_t;

typedef struct {
    sw_state_t state;
    uint32_t   start_tick;       /* tick at which the current run started */
    uint32_t   accumulated_ms;   /* time accumulated across previous runs */
    uint32_t   last_lap_ms;
    uint8_t    lap_count;

    lv_obj_t  *bezel_arc;
    lv_obj_t  *time_label;
    lv_obj_t  *lap_label;
    lv_obj_t  *primary_btn;
    lv_obj_t  *primary_btn_label;
    lv_obj_t  *secondary_btn;
    lv_obj_t  *secondary_btn_label;
    lv_obj_t  *lap_list;

    lv_timer_t *refresh_timer;
} stopwatch_t;

static stopwatch_t s_sw;

static uint32_t elapsed_ms(const stopwatch_t *sw)
{
    if (sw->state == SW_STATE_RUNNING) {
        return sw->accumulated_ms + (lv_tick_get() - sw->start_tick);
    }
    return sw->accumulated_ms;
}

static void format_time(uint32_t ms, char *buf, size_t buf_len)
{
    uint32_t minutes      = ms / 60000U;
    uint32_t seconds      = (ms / 1000U) % 60U;
    uint32_t centiseconds = (ms / 10U) % 100U;
    snprintf(buf, buf_len, "%02u:%02u.%02u",
             (unsigned)minutes, (unsigned)seconds, (unsigned)centiseconds);
}

static void refresh_labels(stopwatch_t *sw)
{
    char buf[16];
    uint32_t ms = elapsed_ms(sw);

    format_time(ms, buf, sizeof buf);
    lv_label_set_text(sw->time_label, buf);

    /* Bezel arc: one full sweep per minute */
    uint32_t arc_val = (ms % 60000U) * 360U / 60000U;
    lv_arc_set_value(sw->bezel_arc, (int32_t)arc_val);
}

static void refresh_timer_cb(lv_timer_t *t)
{
    (void)t;
    refresh_labels(&s_sw);
}

static void update_buttons(stopwatch_t *sw)
{
    switch (sw->state) {
    case SW_STATE_IDLE:
        lv_label_set_text(sw->primary_btn_label, "Start");
        lv_label_set_text(sw->secondary_btn_label, "Reset");
        lv_obj_add_state(sw->secondary_btn, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(sw->primary_btn,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
        break;
    case SW_STATE_RUNNING:
        lv_label_set_text(sw->primary_btn_label, "Pause");
        lv_label_set_text(sw->secondary_btn_label, "Lap");
        lv_obj_clear_state(sw->secondary_btn, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(sw->primary_btn,
                                  lv_palette_main(LV_PALETTE_RED), 0);
        break;
    case SW_STATE_PAUSED:
        lv_label_set_text(sw->primary_btn_label, "Resume");
        lv_label_set_text(sw->secondary_btn_label, "Reset");
        lv_obj_clear_state(sw->secondary_btn, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(sw->primary_btn,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
        break;
    }
}

static void clear_laps(stopwatch_t *sw)
{
    lv_obj_clean(sw->lap_list);
    sw->lap_count   = 0;
    sw->last_lap_ms = 0;
    lv_label_set_text(sw->lap_label, "");
}

static void add_lap(stopwatch_t *sw)
{
    uint32_t now_ms  = elapsed_ms(sw);
    uint32_t diff_ms = now_ms - sw->last_lap_ms;
    sw->last_lap_ms  = now_ms;
    sw->lap_count++;

    char split[16];
    char total[16];
    format_time(diff_ms, split, sizeof split);
    format_time(now_ms,  total, sizeof total);

    char line[48];
    snprintf(line, sizeof line, "L%02u  %s", sw->lap_count, split);

    lv_obj_t *row = lv_label_create(sw->lap_list);
    lv_label_set_text(row, line);
    lv_obj_set_style_text_color(row, lv_color_white(), 0);
    lv_obj_set_style_text_font(row, &lv_font_montserrat_14, 0);

    char summary[40];
    snprintf(summary, sizeof summary, "Last lap %s", split);
    lv_label_set_text(sw->lap_label, summary);
}

static void primary_btn_event_cb(lv_event_t *e)
{
    (void)e;
    stopwatch_t *sw = &s_sw;

    switch (sw->state) {
    case SW_STATE_IDLE:
        sw->state      = SW_STATE_RUNNING;
        sw->start_tick = lv_tick_get();
        break;
    case SW_STATE_RUNNING:
        sw->accumulated_ms += (lv_tick_get() - sw->start_tick);
        sw->state           = SW_STATE_PAUSED;
        break;
    case SW_STATE_PAUSED:
        sw->state      = SW_STATE_RUNNING;
        sw->start_tick = lv_tick_get();
        break;
    }
    update_buttons(sw);
    refresh_labels(sw);
}

static void secondary_btn_event_cb(lv_event_t *e)
{
    (void)e;
    stopwatch_t *sw = &s_sw;

    if (sw->state == SW_STATE_RUNNING) {
        add_lap(sw);
        return;
    }

    /* IDLE or PAUSED -> Reset */
    sw->state          = SW_STATE_IDLE;
    sw->accumulated_ms = 0;
    sw->start_tick     = 0;
    clear_laps(sw);
    update_buttons(sw);
    refresh_labels(sw);
}

static void build_background(lv_obj_t *scr)
{
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    /* Round dial container — emulates the watch's circular bezel. */
    lv_obj_t *dial = lv_obj_create(scr);
    lv_obj_set_size(dial, SCREEN_SIZE - RING_PAD, SCREEN_SIZE - RING_PAD);
    lv_obj_center(dial);
    lv_obj_set_style_radius(dial, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dial, lv_color_hex(0x101010), 0);
    lv_obj_set_style_border_color(dial, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(dial, 2, 0);
    lv_obj_set_style_pad_all(dial, 0, 0);
    lv_obj_clear_flag(dial, LV_OBJ_FLAG_SCROLLABLE);
}

static void build_bezel_arc(lv_obj_t *scr, stopwatch_t *sw)
{
    lv_obj_t *arc = lv_arc_create(scr);
    lv_obj_set_size(arc, SCREEN_SIZE - 24, SCREEN_SIZE - 24);
    lv_obj_center(arc);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_range(arc, 0, 360);
    lv_arc_set_value(arc, 0);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_arc_color(arc, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_CYAN),
                               LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);

    sw->bezel_arc = arc;
}

static void build_time_label(lv_obj_t *scr, stopwatch_t *sw)
{
    lv_obj_t *label = lv_label_create(scr);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "00:00.00");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);
    sw->time_label = label;

    lv_obj_t *lap = lv_label_create(scr);
    lv_obj_set_style_text_font(lap, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(lap, lv_color_hex(0x8a8a8a), 0);
    lv_label_set_text(lap, "");
    lv_obj_align(lap, LV_ALIGN_CENTER, 0, 4);
    sw->lap_label = lap;
}

static lv_obj_t *make_round_button(lv_obj_t *parent, lv_color_t bg_color)
{
    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_set_size(btn, 110, 60);
    lv_obj_set_style_radius(btn, 30, 0);
    lv_obj_set_style_bg_color(btn, bg_color, 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    return btn;
}

static void build_buttons(lv_obj_t *scr, stopwatch_t *sw)
{
    /* Primary (Start / Pause / Resume) on the right */
    sw->primary_btn = make_round_button(scr, lv_palette_main(LV_PALETTE_GREEN));
    lv_obj_align(sw->primary_btn, LV_ALIGN_BOTTOM_MID, 60, -60);
    lv_obj_add_event_cb(sw->primary_btn, primary_btn_event_cb,
                        LV_EVENT_CLICKED, NULL);
    sw->primary_btn_label = lv_label_create(sw->primary_btn);
    lv_obj_set_style_text_font(sw->primary_btn_label,
                               &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(sw->primary_btn_label, lv_color_white(), 0);
    lv_label_set_text(sw->primary_btn_label, "Start");
    lv_obj_center(sw->primary_btn_label);

    /* Secondary (Lap / Reset) on the left */
    sw->secondary_btn = make_round_button(scr, lv_color_hex(0x404040));
    lv_obj_align(sw->secondary_btn, LV_ALIGN_BOTTOM_MID, -60, -60);
    lv_obj_add_event_cb(sw->secondary_btn, secondary_btn_event_cb,
                        LV_EVENT_CLICKED, NULL);
    sw->secondary_btn_label = lv_label_create(sw->secondary_btn);
    lv_obj_set_style_text_font(sw->secondary_btn_label,
                               &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(sw->secondary_btn_label, lv_color_white(), 0);
    lv_label_set_text(sw->secondary_btn_label, "Reset");
    lv_obj_center(sw->secondary_btn_label);
    lv_obj_add_state(sw->secondary_btn, LV_STATE_DISABLED);
}

static void build_lap_list(lv_obj_t *scr, stopwatch_t *sw)
{
    lv_obj_t *list = lv_obj_create(scr);
    lv_obj_set_size(list, 200, 70);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_pad_all(list, 2, 0);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(list, LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    sw->lap_list = list;
}

void stopwatch_ui_create(void)
{
    stopwatch_t *sw    = &s_sw;
    sw->state          = SW_STATE_IDLE;
    sw->start_tick     = 0;
    sw->accumulated_ms = 0;
    sw->last_lap_ms    = 0;
    sw->lap_count      = 0;

    lv_obj_t *scr = lv_screen_active();

    build_background(scr);
    build_bezel_arc(scr, sw);
    build_lap_list(scr, sw);
    build_time_label(scr, sw);
    build_buttons(scr, sw);
    update_buttons(sw);
    refresh_labels(sw);

    sw->refresh_timer = lv_timer_create(refresh_timer_cb, 30, NULL);
}
