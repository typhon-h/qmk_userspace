
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
#define PERKEY_NUM 42
#define TOTAL_LEDS (UNDERGLOW_NUM + PERKEY_NUM)
int underglow_leds[UNDERGLOW_NUM];

void rgb_keyboard_post_init_user(void);

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record);

void housekeeping_task_rgb(void);

void     rgb_timer_reset(void);

void rgb_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);

enum layers { BASE, EXTRA, TAP, BUTTON, NAV, MOUSE, MEDIA, NUM, SYM, FUN, UNKNOWN, NUM_LAYERS };


//TODO: manage all of this better

#define HSV_TRANS 1, 1, 1
int nav_layer_led[TOTAL_LEDS][3];

extern int num_layer_led[TOTAL_LEDS][3];

extern int media_layer_led[TOTAL_LEDS][3];

extern int mouse_layer_led[TOTAL_LEDS][3];

extern int sym_layer_led[TOTAL_LEDS][3];

extern int default_layer_led[TOTAL_LEDS][3];
