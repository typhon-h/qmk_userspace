
#pragma once

#include "color.h"

typedef struct {
    bool is_timeout;
    bool is_forced_off;
    bool is_recording;
} rgb_state_t;

extern uint32_t rgb_timer;

extern uint8_t rgb_mode;
extern uint8_t rgb_hue;
extern uint8_t rgb_val;

void rgb_keyboard_post_init_user(void);

bool rgb_process_keycode(uint16_t keycode, keyrecord_t *record);

void housekeeping_task_rgb(void);

void     rgb_timer_reset(void);

void rgb_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);

#define HSV_TRANS 1, 1, 1
#define UNDERGLOW_NUM 12
#define PERKEY_NUM 42
#define NUM_LEDS (UNDERGLOW_NUM + PERKEY_NUM)

#define LED_MAP( \
            L02,  L01,  L00,                            L27,  L28, L29,           \
            L03,  L04,  L05,                            L32,  L31, L30,           \
      L24,  L23,  L18,  L17,  L10,  L09,          L36,  L37,  L44, L45, L50, L51, \
      L25,  L22,  L19,  L16,  L11,  L08,          L35,  L38,  L43, L46, L49, L52, \
      L26,  L21,  L20,  L15,  L12,  L07,          L34,  L39,  L42, L47, L48, L53, \
                        L14,  L13,  L06,          L33,  L40,  L41                 \
) \
{ \
L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, \
L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, \
L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, \
L30, L31, L32, L33, L34, L35, L36, L37, L38, L39, \
L40, L41, L42, L43, L44, L45, L46, L47, L48, L49, \
L50, L51, L52, L53 \
}


static const int underglow_leds[UNDERGLOW_NUM] = {0, 1, 2, 3, 4, 5, 27, 28, 29, 30, 31, 32};

#define NAV_LAYER_LED LED_MAP(                                                                                                                                                            \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                           \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                           \
      {HSV_OFF},  {HSV_WHITE}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_OFF},          {HSV_WHITE},     {HSV_BLUE},      {HSV_BLUE},    {HSV_BLUE},    {HSV_WHITE},   {HSV_OFF}, \
      {HSV_OFF},  {HSV_GREEN}, {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_OFF},          {HSV_RED},       {HSV_YELLOW},    {HSV_YELLOW},  {HSV_YELLOW},  {HSV_YELLOW},  {HSV_OFF}, \
      {HSV_OFF},  {HSV_OFF},   {HSV_OFF},     {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},          {HSV_RED},       {HSV_MAGENTA},   {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_OFF}, \
                                              {HSV_OFF},     {HSV_OFF},     {HSV_OFF},          {HSV_GOLDENROD}, {HSV_GOLDENROD}, {HSV_GOLDENROD}                                         \
);

#define NUM_LAYER_LED LED_MAP(                                                                                                                                             \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                    \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                    \
      {HSV_OFF},  {HSV_CYAN},   {HSV_RED},  {HSV_RED},   {HSV_RED},  {HSV_CYAN},          {HSV_OFF},  {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_WHITE}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_YELLOW}, {HSV_RED},  {HSV_RED},   {HSV_RED},  {HSV_MAGENTA},       {HSV_OFF},  {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_YELLOW}, {HSV_RED},  {HSV_RED},   {HSV_RED},  {HSV_MAGENTA},       {HSV_OFF},  {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},     {HSV_OFF},   {HSV_OFF}, \
                                            {HSV_WHITE}, {HSV_RED},  {HSV_MAGENTA},       {HSV_OFF},  {HSV_OFF},     {HSV_OFF}                                             \
);

#define MEDIA_LAYER_LED LED_MAP(                                                                                                                                                        \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                         \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                         \
      {HSV_OFF},  {HSV_WHITE}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_OFF},          {HSV_PINK},      {HSV_PINK},      {HSV_PINK}, {HSV_PINK}, {HSV_SPRINGGREEN}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_GREEN}, {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_OFF},          {HSV_MAGENTA},   {HSV_GREEN},     {HSV_BLUE}, {HSV_BLUE}, {HSV_GREEN},       {HSV_OFF}, \
      {HSV_OFF},  {HSV_OFF},   {HSV_OFF},     {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},          {HSV_OFF},       {HSV_OFF},       {HSV_OFF},  {HSV_OFF},  {HSV_OFF},         {HSV_OFF}, \
                                              {HSV_OFF},     {HSV_OFF},     {HSV_OFF},          {HSV_GOLDENROD}, {HSV_GOLDENROD}, {HSV_BLUE}                                                                                                                                                                                                    \
);

#define MOUSE_LAYER_LED LED_MAP(                                                                                                                                                                    \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                                     \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                                    {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                                     \
      {HSV_OFF},  {HSV_WHITE}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_OFF},          {HSV_WHITE},     {HSV_BLUE},      {HSV_BLUE},    {HSV_BLUE},    {HSV_WHITE},   {HSV_OFF},           \
      {HSV_OFF},  {HSV_GREEN}, {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_OFF},          {HSV_OFF},       {HSV_YELLOW},    {HSV_YELLOW},  {HSV_YELLOW},  {HSV_YELLOW},  {HSV_OFF},           \
      {HSV_OFF},  {HSV_OFF},   {HSV_OFF},     {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},          {HSV_OFF},       {HSV_MAGENTA},   {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_OFF},           \
                                              {HSV_OFF},     {HSV_OFF},     {HSV_OFF},          {HSV_RED},       {HSV_RED},       {HSV_RED}                                                         \
);

#define SYM_LAYER_LED LED_MAP(                                                                                                                                               \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                      \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                      \
      {HSV_OFF},  {HSV_YELLOW}, {HSV_BLUE},  {HSV_BLUE},  {HSV_WHITE}, {HSV_YELLOW},        {HSV_OFF},  {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_WHITE}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_RED},    {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_MAGENTA},       {HSV_OFF},  {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_RED},    {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_RED},           {HSV_OFF},  {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},     {HSV_OFF},   {HSV_OFF}, \
                                             {HSV_WHITE}, {HSV_WHITE}, {HSV_MAGENTA},       {HSV_OFF},  {HSV_OFF},     {HSV_OFF}                                             \
);

#define FUN_LAYER_LED LED_MAP(                                                                                                                                               \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                      \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                      \
      {HSV_OFF},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_GREEN},          {HSV_OFF},  {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_MAGENTA}, {HSV_WHITE}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_OFF},            {HSV_OFF},  {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN},   {HSV_GREEN}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_OFF},            {HSV_OFF},  {HSV_BLUE},    {HSV_BLUE},    {HSV_OFF},     {HSV_OFF},   {HSV_OFF}, \
                                            {HSV_OFF},  {HSV_WHITE},  {HSV_WHITE},          {HSV_OFF},  {HSV_OFF},     {HSV_OFF}                                             \
);

#define BUTTON_LAYER_LED LED_MAP(                                                                                                                                        \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                  \
                            {HSV_TRANS},  {HSV_TRANS},  {HSV_TRANS},                            {HSV_TRANS},  {HSV_TRANS}, {HSV_TRANS},                                  \
      {HSV_OFF},  {HSV_WHITE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_WHITE},          {HSV_WHITE}, {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_WHITE}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_GREEN},  {HSV_GREEN}, {HSV_GREEN}, {HSV_GREEN}, {HSV_MAGENTA},        {HSV_OFF},   {HSV_GREEN}, {HSV_GREEN}, {HSV_GREEN}, {HSV_GREEN}, {HSV_OFF}, \
      {HSV_OFF},  {HSV_WHITE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_WHITE},          {HSV_WHITE}, {HSV_BLUE},  {HSV_BLUE},  {HSV_BLUE},  {HSV_WHITE}, {HSV_OFF}, \
                                             {HSV_RED},   {HSV_RED},   {HSV_RED},            {HSV_RED},   {HSV_RED},   {HSV_RED}                                         \
);
