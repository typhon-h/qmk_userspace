SPLIT_KEYBOARD = yes
CONVERT_TO = helios

OLED_ENABLE = yes
SERIAL_DRIVER = vendor
# Custom OLED options
OCEAN_DREAM_ENABLE = yes
KEYBOARD_PET_ENABLE = yes
LAYER_LABEL_ENABLE = yes

RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes

WPM_ENABLE = yes

# Layout specific keys
DYNAMIC_MACRO_ENABLE = yes
KEY_LOCK_ENABLE = yes

# RGB code
ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    SRC += $(KEYMAP_PATH)/rgb.c
endif

# OLED code
ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += $(KEYMAP_PATH)/oled.c

    ifdef OCEAN_DREAM_ENABLE
        ifeq ($(strip $(OCEAN_DREAM_ENABLE)), yes)
            SRC += oled/ocean_dream.c
            OPT_DEFS += -DOCEAN_DREAM_ENABLE
        endif
    endif

	ifdef KEYBOARD_PET_ENABLE
        ifeq ($(strip $(KEYBOARD_PET_ENABLE)), yes)
            SRC += oled/keyboard_pet.c
            OPT_DEFS += -DKEYBOARD_PET_ENABLE
        endif
    endif

    ifdef LAYER_LABEL_ENABLE
        ifeq ($(strip $(LAYER_LABEL_ENABLE)), yes)
            SRC += oled/layer_label.c
            OPT_DEFS += -DLAYER_LABEL_ENABLE
        endif
    endif
endif