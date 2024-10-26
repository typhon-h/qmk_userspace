
#pragma once

uint32_t oled_timer;

void     oled_timer_reset(void);


typedef struct {
    bool is_timeout;
    bool is_forced_off;
} oled_state_t;

extern oled_state_t oled_state;
extern  uint32_t oled_timer;

void print_slave(void);

void print_master(void);

void oled_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);

bool oled_process_keycode(uint16_t keycode, keyrecord_t* record);

void housekeeping_task_oled(void);

void oled_keyboard_post_init_user(void);

void oled_wipe(void);