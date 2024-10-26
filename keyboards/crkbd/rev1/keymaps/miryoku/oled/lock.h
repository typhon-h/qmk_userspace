#pragma once

#include QMK_KEYBOARD_H

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
#define ANIM_SIZE 128           // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024
#define ANIM_OFFSET 1           // How far up/down should the animation move
#define ANIM_START 6            // Where should the animation start (5 is center)

void render_lock(void);