#pragma once

#include QMK_KEYBOARD_H

#define LAYER_LABEL_SIZE 512

enum layers { BASE, EXTRA, TAP, BUTTON, NAV, MOUSE, MEDIA, NUM, SYM, FUN, UNKNOWN, NUM_LAYERS };

void render_label(void);