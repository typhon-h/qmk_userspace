
#include QMK_KEYBOARD_H
#include "rgb.h"
#include "color.h"
#include "transactions.h"

rgb_state_t rgb_state = {
    .is_timeout = false,
    .is_forced_off = true,
    .is_recording = false
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

#ifdef DYNAMIC_MACRO_ENABLE

bool isRecordingLedOn = false;
uint32_t macro_led_timer;

bool dynamic_macro_record_start_user(int8_t direction) {
    rgb_state.is_recording = true;

    return false;
}

bool dynamic_macro_record_end_user(int8_t direction) {
    rgb_state.is_recording = false;
    isRecordingLedOn = false;

    return false;
}
#endif

bool rgb_matrix_indicators_user(void) 
{	

    int (*layout)[3];
    bool is_perkey_layer = true;
    switch (get_highest_layer(layer_state)) {
        case NAV:
            layout = (int[NUM_LEDS][3]) NAV_LAYER_LED;
            break;
        case MOUSE:
            layout = (int[NUM_LEDS][3]) MOUSE_LAYER_LED;
            break;
        case MEDIA:
            layout = (int[NUM_LEDS][3]) MEDIA_LAYER_LED;
            break;
        case NUM:
            layout = (int[NUM_LEDS][3]) NUM_LAYER_LED;
            break;
        case SYM:
            layout = (int[NUM_LEDS][3]) SYM_LAYER_LED;
            break;
        case BUTTON:
            layout = (int[NUM_LEDS][3]) BUTTON_LAYER_LED;
            break;
        case FUN:
            layout = (int[NUM_LEDS][3]) FUN_LAYER_LED;
            break;
        default:
            is_perkey_layer = false;
            break;
    }

    if(is_perkey_layer) {
        for(int i = 0; i < NUM_LEDS; i++) {
        int led_trans[3] = {HSV_TRANS};
        if (memcmp(layout[i], led_trans, sizeof(layout[i]))) {
            RGB rgb = hsv_to_rgb((HSV){layout[i][0], layout[i][1], (layout[i][2] < rgb_matrix_get_val())? layout[i][2] : rgb_matrix_get_val()});
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    }

    #ifdef DYNAMIC_MACRO_ENABLE
    if (rgb_state.is_recording)
    {
        if (timer_elapsed32(macro_led_timer) > 500) 
        {
            isRecordingLedOn = !isRecordingLedOn;
            macro_led_timer = timer_read32();
        }
        if (isRecordingLedOn)
        {
            HSV hsv = {.h = 0, .s = 255, .v = rgb_matrix_get_val()};
            RGB rgb = hsv_to_rgb(hsv);
            for(int i = 0; i < UNDERGLOW_NUM; i++) {
                rgb_matrix_set_color(underglow_leds[i], rgb.r, rgb.b, rgb.g);
            }
        }
    } else {
        macro_led_timer = timer_read32();
    }
    #endif




    return false;
}