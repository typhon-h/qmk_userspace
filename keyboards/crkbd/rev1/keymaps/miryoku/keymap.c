// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H
#include "custom_keycodes.h"
#include "rgb.h"
#include "oled.h"
#include "transactions.h"


void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        #ifdef OLED_ENABLE
            housekeeping_task_oled();
        #endif

        #ifdef RGB_MATRIX_ENABLE
            housekeeping_task_rgb();
        #endif
    }
}

void keyboard_post_init_user(void) {
    #ifdef OLED_ENABLE
        oled_keyboard_post_init_user();
    #endif

    #ifdef RGB_MATRIX_ENABLE
        rgb_keyboard_post_init_user();
    #endif
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        #ifdef OLED_ENABLE
            oled_timer_reset();
        #endif
    }

  return true
    #ifdef RGB_MATRIX_ENABLE
    && rgb_process_keycode(keycode, record)
    #endif

    #ifdef OLED_ENABLE
    && oled_process_keycode(keycode, record)
    #endif
  ;
}