#pragma once
#include "quantum.h"
#include <stdbool.h>

/* settings */
#    define MIN_WALK_SPEED 10
#    define MIN_RUN_SPEED 40

/* advanced settings */
#    define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
#    define ANIM_SIZE 128            // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024
#    define ANIM_FRAMES 2           // number of frames in animation

typedef struct {
    char sit[2][ANIM_SIZE];
    char walk[2][ANIM_SIZE];
    char run[2][ANIM_SIZE];
    char sneak[2][ANIM_SIZE];
} PET_ANIMATION_T;

typedef enum { LUNA, SQUIRTLE, CHARMANDER, BULBASAUR, NUM_PETS } PET_T;

void render_pet(int PET_X, int PET_Y, PET_T pet);
PET_T next_pet(PET_T curr_pet, bool is_forward);

extern bool isSneaking;