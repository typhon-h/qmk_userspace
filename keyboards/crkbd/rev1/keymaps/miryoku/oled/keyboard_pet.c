
#include QMK_KEYBOARD_H
#include "keyboard_pet.h"
#include "../custom_util.h"
#include "../oled.h"

/* timers */
uint32_t anim_timer = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int current_wpm = 0;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

// Max height should be 32px
static const PET_ANIMATION_T PETS[NUM_PETS] PROGMEM = {
    // Can persist up to 8 in eeprom
    [LUNA]       = {.sit =
                        {/* 'sit1', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   },

                   /* 'sit2', 32x32px */
                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .walk =
                        {/* 'walk1', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   },

                   /* 'walk2', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   }},
                    .run =
                        {/* 'run1', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                   },

                   /* 'run2', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   }},
                    .sneak =
                        {/* 'sneak1', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                   },

                   /* 'sneak2', 32x32px */
                   {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                   }}},
    [SQUIRTLE]   = {.sit =
                        {/* 'sit1', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x7e, 0x7f, 0xef, 0xbf, 0xbf, 0xde, 0x7e, 0x38, 0xc4, 0xe8, 0x98, 0x70, 0xc0, 0x00, 0xc0, 0xe0, 0xf8, 0xfc, 0x1c, 0xec, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02, 0x07, 0x09, 0x16, 0x0f, 0x06, 0x11, 0x1f, 0x08, 0x06, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00},

                       /* 'sit2', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xfc, 0xfe, 0xde, 0x7e, 0x7e, 0xbc, 0x7c, 0x38, 0xc4, 0xe8, 0x98, 0x70, 0xc0, 0x00, 0xe0, 0xfe, 0xff, 0x47, 0x3b, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x07, 0x09, 0x16, 0x0f, 0x06, 0x11, 0x1f, 0x08, 0x06, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .walk =
                        {/* 'walk1', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x7e, 0x7f, 0xef, 0x9f, 0x8f, 0xfe, 0x7e, 0xb8, 0x44, 0xe8, 0x98, 0x70, 0xe0, 0x00, 0xe0, 0xfc, 0xfe, 0x8e, 0x36, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x0f, 0x09, 0x16, 0x0f, 0x66, 0x31, 0x68, 0x0e, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                       /* 'walk2', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x7e, 0x7f, 0xef, 0x9f, 0xdf, 0xfe, 0x7e, 0xb8, 0x44, 0xe8, 0x98, 0x70, 0xc8, 0x3e, 0xff, 0x07, 0x1b, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x0f, 0x19, 0x16, 0x6f, 0x26, 0x71, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .run =
                        {/* 'run1', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x06, 0xfc, 0x60, 0xe4, 0xe4, 0x40, 0x58, 0xf0, 0x1c, 0x3f, 0x6f, 0xbf, 0xff, 0x87, 0x97, 0x77, 0xbc, 0xc0, 0x74, 0xe8, 0x98, 0x70, 0xe0, 0x10, 0x7c, 0xfe, 0x8e, 0x36, 0x14, 0x00, 0x00, 0x19, 0x0c, 0x07, 0x01, 0x02, 0x12, 0x00, 0x03, 0x01, 0x03, 0x0c, 0x10, 0x0c, 0x03, 0x01, 0x07, 0x0f, 0x09, 0x16, 0x0f, 0x66, 0x31, 0x68, 0x0e, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00},

                       /* 'run2', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0xfe, 0xf2, 0x42, 0x60, 0xa8, 0xb8, 0xf0, 0xcc, 0x40, 0x9e, 0xfb, 0x9e, 0x2e, 0x77, 0xff, 0x87, 0x97, 0xf7, 0x7c, 0x80, 0xf0, 0x74, 0x68, 0x98, 0x74, 0xee, 0x0f, 0x07, 0x1b, 0x1a, 0x00, 0x0f, 0x07, 0x39, 0x01, 0x01, 0x04, 0x06, 0x03, 0x04, 0x08, 0x07, 0x7b, 0x88, 0x80, 0x7c, 0x07, 0x40, 0x01, 0x07, 0x0f, 0x0f, 0x1c, 0x0b, 0x77, 0x36, 0x68, 0x0e, 0x00, 0x40, 0x30, 0x00, 0x00}},
                    .sneak =
                        {/* 'sneak1', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x08, 0x10, 0x01, 0x06, 0x00, 0x00, 0x26, 0x21, 0x60, 0xc0, 0xa0, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0e, 0x1e, 0x1e, 0x3b, 0x31, 0x33, 0x1e, 0x0d, 0x13, 0x0c, 0x00, 0x1e, 0x1f, 0x07, 0x1b, 0x0a, 0x00, 0x00, 0x00},

                       /* 'sneak2', 32x32px */
                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x88, 0x88, 0xd8, 0x70, 0xe8, 0x98, 0x70, 0xe0, 0x00, 0x00, 0x18, 0x60, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0e, 0x0c, 0x0c, 0x07, 0x03, 0x04, 0x03, 0x00, 0x0c, 0x1c, 0x3e, 0x0e, 0x36, 0x0c, 0x00, 0x00}}},
    [CHARMANDER] = {.sit =
                        {/* 'sit1', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0xd0, 0xb0, 0x00, 0x0d, 0x0b, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7c, 0x7f, 0xff, 0xf7, 0xdf, 0xdf, 0xef, 0xfc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x3f, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x07, 0x09, 0x14, 0x17, 0x1b, 0x1f, 0x1e, 0x0c, 0x00, 0x18, 0x1c, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                         /* 'sit2', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x68, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xf8, 0xfe, 0xfe, 0xef, 0xbf, 0xbf, 0xde, 0xf8, 0x80, 0x00, 0x00, 0x06, 0x05, 0x00, 0x00, 0x00, 0x18, 0xff, 0x7e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x09, 0x07, 0x09, 0x14, 0x17, 0x1b, 0x1f, 0x1e, 0x0c, 0x00, 0x18, 0x18, 0x0c, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .walk =
                        {/* 'walk1', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7c, 0x7f, 0xff, 0xef, 0xcf, 0xc7, 0xff, 0xfc, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0xff, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x02, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                         /* 'walk2', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xf8, 0xfe, 0xfe, 0xbf, 0xbf, 0xdf, 0xfe, 0xf8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1f, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x00, 0x0c, 0x0e, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .run =
                        {/* 'run1', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0xfe, 0xfc, 0xf8, 0xf9, 0xf8, 0xf0, 0xf8, 0xf0, 0xfc, 0x6f, 0x67, 0x67, 0xdf, 0xcf, 0xcb, 0xf9, 0xf9, 0xff, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xbf, 0xff, 0x1f, 0x0f, 0x00, 0x00, 0x03, 0x07, 0x17, 0x03, 0x01, 0x01, 0x09, 0x00, 0x01, 0x00, 0x01, 0x06, 0x08, 0x06, 0x01, 0x10, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x02, 0x07, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00},

                         /* 'run2', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xf0, 0xf0, 0xb8, 0x58, 0x88, 0x00, 0xfe, 0xfd, 0xf0, 0xf2, 0xf0, 0xfc, 0xf8, 0xf0, 0xc2, 0xe0, 0xfe, 0xfd, 0xce, 0xcf, 0xbf, 0xff, 0x97, 0xf3, 0xf3, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x0f, 0x1f, 0x3f, 0xff, 0x3f, 0x1f, 0x0f, 0x00, 0x0f, 0x07, 0x13, 0x03, 0x03, 0x07, 0x17, 0x03, 0x01, 0x09, 0x07, 0x39, 0x40, 0x40, 0x39, 0x07, 0x13, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x02, 0x0f, 0x07, 0x40, 0x30, 0x00, 0x00}},
                    .sneak =
                        {/* 'sneak1', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x06, 0x0c, 0x06, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x60, 0xc0, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x30, 0x7c, 0x7f, 0xff, 0xef, 0x8f, 0xaf, 0xff, 0xfc, 0xf0, 0x30, 0x20, 0x00, 0x00, 0x00, 0x8e, 0xff, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x02, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                         /* 'sneak2', 32x32px */
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xf8, 0xfe, 0xfe, 0xdf, 0x9f, 0x8f, 0xfe, 0xf8, 0x81, 0x03, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x1f, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x07, 0x0f, 0x19, 0x14, 0x57, 0x3b, 0x5e, 0x0c, 0x00, 0x0c, 0x0e, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    [BULBASAUR]  = {.sit =
                        {/* 'sit1', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0x10, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x80, 0x30, 0xe0, 0x30, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0xc0, 0xff, 0x3f, 0x04, 0x1c, 0xf8, 0xe8, 0xfc, 0xfc, 0x6f, 0xe6, 0x61, 0x79, 0x76, 0xef, 0xa1, 0xce, 0xbf, 0xbf, 0x9e, 0xc1, 0x9f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x03, 0x01, 0x06, 0x0e, 0x0e, 0x0f, 0x11, 0x1f, 0x0f, 0x07, 0x02, 0x1e, 0x1f, 0x1f, 0x6f, 0x7f, 0x63, 0x21, 0x03, 0x1f, 0x1c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                        /* 'sit2', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf8, 0x1c, 0x1c, 0xf8, 0xe0, 0xf0, 0xf0, 0xbc, 0x98, 0xc4, 0xe6, 0xda, 0xbd, 0x06, 0xb8, 0x7f, 0x7c, 0x7b, 0x86, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0e, 0x0e, 0x0f, 0x11, 0x1f, 0x0f, 0x0e, 0x05, 0x1c, 0x1e, 0x1e, 0x6f, 0x7f, 0x67, 0x23, 0x07, 0x1f, 0x19, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .walk =
                        {/* 'walk1', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf8, 0xf0, 0xb0, 0x90, 0x8c, 0xe6, 0xda, 0xbd, 0x86, 0x38, 0xff, 0xfc, 0x7b, 0x06, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x0f, 0x1f, 0x1d, 0x13, 0x11, 0x1d, 0x1d, 0x6f, 0x7e, 0x67, 0x22, 0x06, 0x1e, 0x19, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                        /* 'walk2', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xe0, 0x60, 0x20, 0x1c, 0x86, 0xba, 0x7d, 0x06, 0xf8, 0xff, 0xfc, 0x7b, 0x06, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1d, 0x1f, 0x3f, 0x3b, 0x2f, 0x27, 0x33, 0x3b, 0x1f, 0x6d, 0x7e, 0x66, 0x22, 0x06, 0x1f, 0x19, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                    .run =
                        {/* 'run1', 32x32px */
                        {0xff, 0xff, 0xfe, 0xfc, 0xfc, 0xf8, 0xf0, 0xf0, 0xe0, 0xc0, 0xc0, 0x80, 0x00, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x5f, 0x1f, 0x9f, 0xde, 0xfc, 0xbc, 0x78, 0x70, 0xe0, 0x58, 0xdd, 0xc8, 0xe2, 0x40, 0x08, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xdf, 0x43, 0x3d, 0x7e, 0x7f, 0xf3, 0xcf, 0x3e, 0x7d, 0x7d, 0x3b, 0x86, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3b, 0x7f, 0x7f, 0x76, 0x5e, 0x4e, 0x67, 0x77, 0xbe, 0x5a, 0x7c, 0x73, 0x31, 0x03, 0x0f, 0x1c, 0x1c, 0x00, 0x40, 0x30, 0x00, 0x00},

                        /* 'run2', 32x32px */
                        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xd8, 0x98, 0x98, 0x18, 0x38, 0x38, 0x30, 0x10, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0xff, 0xdf, 0x9f, 0x3f, 0x7f, 0x7f, 0xff, 0xdf, 0xdf, 0x9f, 0x5f, 0x1f, 0x3f, 0xbf, 0xff, 0x3e, 0xbe, 0x7c, 0x98, 0x62, 0xc0, 0x88, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x03, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xe0, 0xd0, 0xcf, 0x41, 0x3e, 0x7f, 0x7f, 0xf8, 0xc7, 0x3f, 0x7e, 0x7e, 0x3d, 0x83, 0xde, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3b, 0x7f, 0x7f, 0x76, 0x5e, 0x4e, 0x67, 0x77, 0x3e, 0x5a, 0x7c, 0x73, 0x31, 0x03, 0x0f, 0x1c, 0x1c, 0x40, 0x30, 0x00}},
                    .sneak =
                        {/* 'sneak1', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x20, 0x50, 0x00, 0x00, 0x40, 0xe0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0x00, 0x02, 0x07, 0x82, 0x04, 0x60, 0xc0, 0x61, 0x80, 0x00, 0x00, 0x00, 0x20, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xe0, 0x60, 0x20, 0x1e, 0x83, 0xbd, 0x7e, 0x03, 0xfc, 0x7f, 0x7e, 0x3d, 0x83, 0xbe, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1d, 0x3f, 0x3f, 0x3b, 0x2f, 0x27, 0x37, 0x37, 0x1f, 0x6d, 0x7e, 0x73, 0x31, 0x03, 0x0f, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                        /* 'sneak2', 32x32px */
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xe0, 0x60, 0x80, 0x40, 0x00, 0x88, 0x1c, 0x28, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x0a, 0xc4, 0x20, 0x24, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0xc0, 0x00, 0x00, 0x20, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xe0, 0x60, 0x20, 0x1c, 0xc6, 0xba, 0x7d, 0x06, 0xf8, 0xff, 0xfc, 0x7b, 0x06, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1d, 0x3f, 0x3f, 0x3b, 0x2f, 0x27, 0x33, 0x3b, 0x1f, 0x6d, 0x7e, 0x66, 0x22, 0x06, 0x0f, 0x19, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
};

PET_T next_pet(PET_T curr_pet, bool is_forward) {
    is_forward ? curr_pet++ : curr_pet--;
    return curr_pet % NUM_PETS;
}

/* animation */
void animate_pet(int PET_X, int PET_Y, PET_T pet) {
    /* clear */
    oled_set_cursor(PET_X, PET_Y - 1);
    oled_write("     ", false);

    oled_set_cursor(PET_X, PET_Y);

    /* switch frame */
    current_frame = (current_frame + 1) % 2;

    /* current status */
    if (get_highest_layer(layer_state) != BASE) {
        oled_write_raw_P(PETS[pet].sneak[current_frame], ANIM_SIZE);

    } else if (current_wpm <= MIN_WALK_SPEED) {
        oled_write_raw_P(PETS[pet].sit[current_frame], ANIM_SIZE);

    } else if (current_wpm <= MIN_RUN_SPEED) {
        oled_write_raw_P(PETS[pet].walk[current_frame], ANIM_SIZE);

    } else {
        oled_write_raw_P(PETS[pet].run[current_frame], ANIM_SIZE);
    }
}

/* logic */
void render_pet(int PET_X, int PET_Y, PET_T pet) {
    /* the animation prevents the normal timeout from occuring */
    if (oled_state.is_forced_off || oled_state.is_timeout) {
        if (is_oled_on()) {
            oled_wipe();
        }
        oled_off();
        return;
    }

    /* KEYBOARD PET VARIABLES START */

    current_wpm = get_current_wpm();

    /* KEYBOARD PET VARIABLES END */

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_pet(PET_X, PET_Y, pet);
    }
}
