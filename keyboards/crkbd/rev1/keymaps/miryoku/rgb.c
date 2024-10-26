
#include QMK_KEYBOARD_H
#include "rgb.h"
#include "transactions.h"

rgb_state_t rgb_state = {
    .is_timeout = false,
    .is_forced_off = true
}; // look at merging this with the oled timer stuff as a class

uint32_t rgb_timer = 0;

uint8_t rgb_mode           = 0;
uint8_t rgb_hue            = 0;
uint8_t rgb_val            = 0;

void rgb_keyboard_post_init_user() {
    rgb_state.is_forced_off = !rgb_matrix_is_enabled();
    rgb_mode = rgb_matrix_get_mode();
    rgb_hue = rgb_matrix_get_hue();
    rgb_val = rgb_matrix_get_val();

    transaction_register_rpc(RPC_RGB_SYNC, rgb_sync_handler);
}

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
                rgb_matrix_toggle_noeeprom();
                rgb_state.is_forced_off = !rgb_matrix_is_enabled();
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
        case RGB_VAI: // TODO: maybe use this key to also save oled on/off state to eeprom
            if (record->event.pressed) {

                if (rgb_state.is_forced_off) {
                    rgb_matrix_disable();
                } else {
                    rgb_matrix_enable();
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
    bool            needs_sync = false;
    static uint16_t last_sync  = false;
    static bool     last_state = false;

    rgb_state.is_timeout = timer_elapsed32(rgb_timer) > RGB_TIMEOUT;

    if (timer_elapsed32(last_sync) > 250) {
        needs_sync = true;
    } else if (memcmp(&rgb_state, &last_state, sizeof(last_state))) {
        needs_sync = true;
        memcpy(&last_state, &rgb_state, sizeof(last_state));
    }

    // Perform the sync if requested
    if (needs_sync) {
        if (transaction_rpc_send(RPC_RGB_SYNC, sizeof(rgb_state), &rgb_state)) {
            last_sync = timer_read32();
        }
    }

    if(rgb_state.is_forced_off || rgb_state.is_timeout) {
        rgb_matrix_disable_noeeprom();
    } else {
        rgb_matrix_enable_noeeprom();
    }
}

void rgb_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    memcpy(&rgb_state, in_data, in_buflen);
}

void rgb_timer_reset(void) {
    rgb_timer = timer_read32();
}