
#include QMK_KEYBOARD_H

#include "oled.h"
#include "oled/ocean_dream.h"
#include "oled/keyboard_pet.h"
#include "oled/layer_label.h"
#include "transactions.h"
#include "custom_keycodes.h"

bool is_oled_enabled = true;
bool is_oled_toggled = true;
uint32_t oled_timer = 0;


// setup for persisting the pet. currently only thing using eeprom so leaving it here
// TODO: find better way to handle this
typedef union {
  uint32_t raw;
  struct {
    uint8_t     pet: 3;
  };
} user_config_t;

user_config_t user_config;


void oled_timer_reset(void) {
    oled_timer = timer_read32();
}

uint32_t get_oled_timeout(void) {
    uint32_t timeout = OLED_TIMEOUT;

    return timeout;
}

void oled_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    memcpy(&is_oled_enabled, in_data, in_buflen);
}

void print_slave(void) {
#ifdef OCEAN_DREAM_ENABLE
    render_stars();
#endif
}

void print_master(void) {
    oled_set_cursor(0, 0);

#ifdef LAYER_LABEL_ENABLE
    render_label();
#endif

    oled_set_cursor(0, 7);
    oled_write("     ", false);

    oled_set_cursor(0, 8);
    oled_write("     ", false);

    /* wpm counter */
    uint8_t n = get_current_wpm();
    char    wpm_str[6];
    oled_set_cursor(0, 9);
    wpm_str[5] = '\0';
    wpm_str[3] = wpm_str[4] = ' ';
    wpm_str[2]              = '0' + n % 10;
    wpm_str[1]              = '0' + (n /= 10) % 10;
    wpm_str[0]              = '0' + n / 10;
    oled_write(wpm_str, false);

#ifdef KEYBOARD_PET_ENABLE
    render_pet(0, 12, user_config.pet);
#endif
}

bool oled_task_user(void) {
    if (!is_oled_enabled) {
        oled_clear();
        oled_off();
        return false;
    } else {
        oled_on();
    }

    if (is_keyboard_master()) {
        print_master();
    } else {
        print_slave();
    }
    return false;
}

void housekeeping_task_oled(void) {
    bool            needs_sync = false;
    static uint16_t last_sync  = false;
    static bool     last_state = false;

    if (!is_oled_toggled) {
        is_oled_enabled = false;
    } else {
        is_oled_enabled = !(timer_elapsed32(oled_timer) > get_oled_timeout());
    }

    if (memcmp(&is_oled_enabled, &last_state, sizeof(last_state))) {
        needs_sync = true;
        memcpy(&last_state, &is_oled_enabled, sizeof(last_state));
    }

    if (timer_elapsed32(last_sync) > 250) {
        needs_sync = true;
    }

    // Perform the sync if requested
    if (needs_sync) {
        if (transaction_rpc_send(RPC_OLED_SYNC, sizeof(is_oled_enabled), &is_oled_enabled)) {
            last_sync = timer_read32();
        }
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_process_keycode(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case OLED_EVENT:
            if (record->event.pressed) {
                if(get_mods() & MOD_MASK_GUI) {
                    #ifdef OLED_ENABLE
                        is_oled_toggled = !is_oled_toggled;
                        if (is_oled_toggled) {
                            oled_on();
                        }
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
    case LT(NAV, KC_SPC):
    case KC_SPC:
        #ifdef KEYBOARD_PET_ENABLE
            isJumping = record->event.pressed;
            if (isJumping) {
                showedJump = false;  
            }     
         #endif
        break;
    
    default:
        break;
  }

  return true;
}


void oled_keyboard_post_init_user() {
    transaction_register_rpc(RPC_OLED_SYNC, oled_sync_handler);
    user_config.raw = eeconfig_read_user();
}