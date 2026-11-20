// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "manna-harbour_miryoku.h"

#if defined(MIRYOKU_LEAGUE)

static bool autoshift_before_league;
static bool mode_indicator_ready;
// Keys held through a mode change must be released before they can act again.
static matrix_row_t transition_keys[MATRIX_ROWS];

static void set_mode_color(bool league) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    // HSV hue 30 is gold; zero saturation gives white. Keep brightness low.
    rgblight_sethsv_noeeprom(league ? 30 : 0, league ? 255 : 0, 32);
}

void keyboard_post_init_user(void) {
    mode_indicator_ready = true;
    set_mode_color(false);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // quantum_init invokes this hook before rgblight_init has set up DMA/PIO.
    if (mode_indicator_ready) {
        set_mode_color((state & ((layer_state_t)1 << U_LOL)) != 0);
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint8_t row = record->event.key.row;
    const matrix_row_t key_mask = (matrix_row_t)1 << record->event.key.col;
    if (transition_keys[row] & key_mask) {
        if (!record->event.pressed) {
            transition_keys[row] &= ~key_mask;
        }
        return false;
    }

    if (keycode != U_LOL_TOGGLE) {
        return true;
    }
    if (!record->event.pressed) {
        return false;
    }

    const bool leaving_league = layer_state_is(U_LOL);
    for (uint8_t i = 0; i < MATRIX_ROWS; ++i) {
        transition_keys[i] = matrix_get_row(i);
    }

    if (!leaving_league) {
        autoshift_before_league = get_autoshift_state();
        autoshift_disable();
    }
    caps_word_off();
#ifndef NO_ACTION_ONESHOT
    clear_oneshot_mods();
    clear_oneshot_locked_mods();
    reset_oneshot_layer();
#endif
    clear_keyboard();

    if (leaving_league) {
        layer_clear();
        if (autoshift_before_league) {
            autoshift_enable();
        }
    } else {
        layer_move(U_LOL);
    }
    return false;
}

#endif
