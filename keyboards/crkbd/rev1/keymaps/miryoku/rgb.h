
#pragma once

#ifdef RGB_MATRIX_ENABLE
extern uint8_t rgb_enabled;
extern bool    rgb_should_restore;
extern uint8_t rgb_mode;
extern uint8_t rgb_hue;
extern uint8_t rgb_val;
#endif

void rgb_keyboard_post_init_user(void);

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record);

void housekeeping_task_rgb(void);
