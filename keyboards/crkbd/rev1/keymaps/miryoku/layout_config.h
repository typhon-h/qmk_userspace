
#pragma once

// clang-formt off
#define LAYOUT_miryoku(K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, N30, N31, K32, K33, K34, K35, K36, K37, N38, N39) LAYOUT_split_3x6_3(DM_REC1, K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, DM_PLY1, CW_TOGG, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, QK_REP, QK_LOCK, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, DF(U_BASE), K32, K33, K34, K35, K36, K37)
// clang-format on

// Layer Overrides

// ----------------------------------------------------------------------------
// clang-format off
#define MIRYOKU_LAYERMAPPING_MEDIA( \
      K00,  K01,  K02,  K03,  K04,                    K05,  K06,  K07,  K08,  K09, \
      K10,  K11,  K12,  K13,  K14,                    K15,  K16,  K17,  K18,  K19, \
      K20,  K21,  K22,  K23,  K24,                    K25,  K26,  K27,  K28,  K29, \
      N30,  N31,  K32,  K33,  K34,                    K35,  K36,  K37,  N38,  N39 \
) \
LAYOUT_split_3x6_3( \
KC_TRNS,  K00,  K01,  K02,  K03,  K04,                       K05,  K06,  K07,  K08,  K09,  KC_TRNS, \
KC_TRNS,  K10,  K11,  K12,  K13,  K14,                OLED_EVENT,  K16,  K17,  K18,  K19,  KC_TRNS, \
KC_TRNS,  K20,  K21,  K22,  K23,  K24,                       K25,  K26,  K27,  K28,  K29,  KC_TRNS , \
                      K32,  K33,  K34,                       K35,  K36,  K37 \
)

// Keep regular QWERTY layout but remove home row mods. TODO: make this layout agnostic
#define MIRYOKU_LAYERMAPPING_EXTRA( \
      K00,  K01,  K02,  K03,  K04,                    K05,  K06,  K07,  K08,  K09, \
      K10,  K11,  K12,  K13,  K14,                    K15,  K16,  K17,  K18,  K19, \
      K20,  K21,  K22,  K23,  K24,                    K25,  K26,  K27,  K28,  K29, \
      N30,  N31,  K32,  K33,  K34,                    K35,  K36,  K37,  N38,  N39 \
) \
LAYOUT_split_3x6_3( \
KC_TRNS,  K00,   K01,   K02,  K03,  K04,                K05,  K06,   K07,  K08,    K09,     KC_TRNS, \
KC_TRNS,  KC_A,  KC_S,  KC_D, KC_F, K14,                K15,  KC_J,  KC_K, KC_L,   KC_QUOT, KC_TRNS, \
KC_TRNS,  KC_Y,  KC_X,  K22,  K23,  K24,                K25,  K26,   K27,  KC_DOT, KC_SLSH, KC_TRNS , \
                        K32,  K33,  K34,                K35,  K36,   K37 \
)
// clang-format on

// ----------------------------------------------------------------------------
