
#include QMK_KEYBOARD_H
#include "rgb.h"
#include "oled.h"

uint8_t rgb_enabled        = 0;
bool    rgb_should_restore = 0;
uint8_t rgb_mode           = 0;
uint8_t rgb_hue            = 0;
uint8_t rgb_val            = 0;

void rgb_keyboard_post_init_user() {
    rgb_enabled = rgb_matrix_is_enabled();
    rgb_should_restore = rgb_enabled;
    rgb_mode = rgb_matrix_get_mode();
    rgb_hue = rgb_matrix_get_hue();
    rgb_val = rgb_matrix_get_val();
}

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
                rgb_matrix_toggle_noeeprom();
                rgb_should_restore = rgb_matrix_is_enabled();
            }
            break;
        case RGB_MOD:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    rgb_matrix_step_reverse_noeeprom();
                } else {
                    rgb_matrix_step_noeeprom();
                }
            }
            break;
        case RGB_HUI:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    rgb_matrix_decrease_hue_noeeprom();
                } else {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            break;
        case RGB_SAI:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    rgb_matrix_decrease_val_noeeprom();
                } else {
                    rgb_matrix_increase_val_noeeprom();
                }
            }
            break;
        case RGB_VAI:
            if (record->event.pressed) {
                if (rgb_enabled != rgb_matrix_is_enabled()) {
                    rgb_enabled = rgb_matrix_is_enabled();
                    rgb_enabled ? rgb_matrix_enable() : rgb_matrix_disable();
                }

                if (rgb_mode != rgb_matrix_get_mode()) {
                    rgb_mode = rgb_matrix_get_mode();
                    rgb_matrix_mode(rgb_mode);
                }

                if (rgb_hue != rgb_matrix_get_hue() || rgb_val != rgb_matrix_get_val()) {
                    rgb_hue = rgb_matrix_get_hue();
                    rgb_val = rgb_matrix_get_val();
                    rgb_matrix_sethsv(rgb_hue, rgb_matrix_get_sat(), rgb_val);
                }
            }
            break;

        default:
            break;
    }
    return true;
}

void housekeeping_task_rgb() {
    // TODO: duplicate the OLED timeout communication for LED timeouts, should fix staying on issue
    // consider disable rgb_should_restore property while making the transition
    if (rgb_should_restore) {
        (is_oled_enabled) ? rgb_matrix_enable_noeeprom() : rgb_matrix_disable_noeeprom();
    } else {
        rgb_matrix_disable_noeeprom();
    }
}