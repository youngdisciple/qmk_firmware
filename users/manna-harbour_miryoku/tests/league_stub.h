// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Host doubles for the QMK services used by the actual league.c handler.
#include <stdbool.h>
#include <stdint.h>
#include "quantum_keycodes.h"

#define KEYBOARD_cheapino
#define MIRYOKU_NAV_VI
#define MIRYOKU_ALPHAS_QWERTY
#include "custom_config.h"

#define MATRIX_ROWS 8
#define MATRIX_COLS 12
#define RGBLIGHT_MODE_STATIC_LIGHT 1
typedef uint16_t matrix_row_t;
typedef uint32_t layer_state_t;
typedef struct {
    struct {
        struct { uint8_t row, col; } key;
        bool pressed;
    } event;
} keyrecord_t;

extern layer_state_t layer_state;
void keyboard_post_init_user(void);
layer_state_t layer_state_set_user(layer_state_t state);
bool process_record_user(uint16_t keycode, keyrecord_t *record);
matrix_row_t matrix_get_row(uint8_t row);
bool layer_state_is(uint8_t layer);
void layer_move(uint8_t layer);
void layer_clear(void);
void rgblight_enable_noeeprom(void);
void rgblight_mode_noeeprom(uint8_t mode);
void rgblight_sethsv_noeeprom(uint8_t hue, uint8_t sat, uint8_t val);
bool get_autoshift_state(void);
void autoshift_disable(void);
void autoshift_enable(void);
void caps_word_off(void);
void clear_oneshot_mods(void);
void clear_oneshot_locked_mods(void);
void reset_oneshot_layer(void);
void clear_keyboard(void);
