
#pragma once

typedef struct {
    bool is_timeout;
    bool is_forced_off;
    bool is_recording;
} rgb_state_t;

extern uint32_t rgb_timer;

extern uint8_t rgb_mode;
extern uint8_t rgb_hue;
extern uint8_t rgb_val;

#define UNDERGLOW_NUM 12
int underglow_leds[UNDERGLOW_NUM];

void rgb_keyboard_post_init_user(void);

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record);

void housekeeping_task_rgb(void);

void     rgb_timer_reset(void);

void rgb_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);
