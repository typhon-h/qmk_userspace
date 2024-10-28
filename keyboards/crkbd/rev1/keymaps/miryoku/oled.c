
#include QMK_KEYBOARD_H

#include "oled.h"
#include "oled/keyboard_pet.h"
#include "oled/layer_label.h"
#include "transactions.h"
#include "custom_keycodes.h"

oled_state_t oled_state = {
    .is_timeout    = false,
    .is_forced_off = false,
};

uint32_t oled_timer = 0;

// setup for persisting the pet. currently only thing using eeprom so leaving it here
// TODO: find better way to handle this
typedef union {
    uint32_t raw;
    struct {
        uint8_t pet : 3;
    };
} user_config_t;

user_config_t user_config;

static const char PROGMEM CORNE_LOGO[] = {
    // 32x32px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0x00, 0x80, 0xf0, 0xfc, 0xfe, 0xfe, 0xff, 0x3f, 0x03, 0xe0, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x03, 0x03, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00};

static const char PROGMEM INITIALS[] = {
    // 'communityIcon_ivpa92rzlg541', 32x32px
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0xf8, 0xf8, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3c, 0x3c, 0x3c, 0xff, 0xff, 0xff, 0xff, 0x3c, 0x3c, 0x3c, 0x3c, 0xfc, 0xfc, 0x3c, 0x3c, 0x3c, 0x3c, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void oled_timer_reset(void) {
    if (!oled_state.is_forced_off) {
        oled_timer = timer_read32();
    }
}

void oled_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    memcpy(&oled_state, in_data, in_buflen);
}

void print_master(void) {
#ifdef LAYER_LABEL_ENABLE
    render_label();
#endif
}

void print_slave(void) {
    oled_set_cursor(0, 0);
    oled_write_raw_P(CORNE_LOGO, sizeof(CORNE_LOGO));
    oled_set_cursor(0, 4);
    oled_write_ln("crkbd", false);
    oled_set_cursor(0, 7);
    oled_write_raw_P(INITIALS, sizeof(INITIALS));

#ifdef KEYBOARD_PET_ENABLE
    render_pet(0, 12, user_config.pet);
#endif
}

bool oled_task_user(void) {
    if (oled_state.is_forced_off || oled_state.is_timeout) {
        if (is_oled_on()) {
            oled_wipe();
        }
        oled_off();
        return false;
    }

    if (is_keyboard_master()) {
        print_master();
    } else {
        print_slave();
    }

    return false;
}

void housekeeping_task_oled(void) {
    bool                needs_sync = false;
    static uint16_t     last_sync  = false;
    static oled_state_t last_state = {};

    oled_state.is_timeout = timer_elapsed32(oled_timer) > OLED_TIMEOUT;

    if (timer_elapsed32(last_sync) > 250) {
        needs_sync = true;
    } else if (memcmp(&oled_state, &last_state, sizeof(last_state))) {
        needs_sync = true;
        memcpy(&last_state, &oled_state, sizeof(last_state));
    }

    // Perform the sync if requested
    if (needs_sync) {
        if (transaction_rpc_send(RPC_OLED_SYNC, sizeof(oled_state), &oled_state)) {
            last_sync = timer_read32();
        }
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_process_keycode(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case OLED_EVENT:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_GUI) {
#ifdef OLED_ENABLE
                    oled_state.is_forced_off = !oled_state.is_forced_off;
                    oled_timer_reset();
#endif
                } else if (get_mods() & MOD_MASK_SHIFT) {
#ifdef KEYBOARD_PET_ENABLE
                    user_config.pet = next_pet(user_config.pet, false);
                    eeconfig_update_user(user_config.raw); // write the setings to EEPROM
#endif
                } else {
#ifdef KEYBOARD_PET_ENABLE
                    user_config.pet = next_pet(user_config.pet, true);
                    eeconfig_update_user(user_config.raw); // write the setings to EEPROM
#endif
                }
            }
            return false; // Skip all further processing of this key

        default:
            break;
    }

    return true;
}

void oled_keyboard_post_init_user() {
    transaction_register_rpc(RPC_OLED_SYNC, oled_sync_handler);
    user_config.raw = eeconfig_read_user();
}

void oled_wipe() {
    oled_set_cursor(0, 0);
    for (int i = 0; i < oled_max_lines(); i++) {
        oled_write_ln(" ", false);
    }
}
