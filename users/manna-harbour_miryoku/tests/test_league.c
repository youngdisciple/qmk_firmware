// SPDX-License-Identifier: GPL-2.0-or-later
#include <assert.h>
#include <stdio.h>
#include "league_stub.h"
#include "manna-harbour_miryoku.h"

// Expand the real layout macros, using real QMK keycodes, into flat arrays.
#define LAYOUT_miryoku(...) {__VA_ARGS__}
#undef TD
#define TD(action) KC_NO
static const uint16_t league[] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_LOL, MIRYOKU_LAYER_LOL);
static const uint16_t league_fn[] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_LOL_FN, MIRYOKU_LAYER_LOL_FN);
static const uint16_t media[] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_MEDIA, MIRYOKU_LAYER_MEDIA);
static const uint16_t normal_fn[] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_FUN, MIRYOKU_LAYER_FUN);

layer_state_t layer_state;
static matrix_row_t matrix[MATRIX_ROWS];
static bool autoshift = true, caps_word, oneshot_mods, locked_mods, oneshot_layer;
static uint8_t held_mods, hue, saturation, brightness;
static unsigned clear_count;
static bool rgb_initialized;

matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }
bool layer_state_is(uint8_t layer) { return (layer_state & (1u << layer)) != 0; }
void layer_move(uint8_t layer) { layer_state = layer_state_set_user(1u << layer); }
void layer_clear(void) { layer_state = layer_state_set_user(0); }
void rgblight_enable_noeeprom(void) { assert(rgb_initialized); }
void rgblight_mode_noeeprom(uint8_t mode) { assert(mode == RGBLIGHT_MODE_STATIC_LIGHT); }
void rgblight_sethsv_noeeprom(uint8_t h, uint8_t s, uint8_t v) { hue = h; saturation = s; brightness = v; }
bool get_autoshift_state(void) { return autoshift; }
void autoshift_disable(void) { autoshift = false; }
void autoshift_enable(void) { autoshift = true; }
void caps_word_off(void) { caps_word = false; }
void clear_oneshot_mods(void) { oneshot_mods = false; }
void clear_oneshot_locked_mods(void) { locked_mods = false; }
void reset_oneshot_layer(void) { oneshot_layer = false; }
void clear_keyboard(void) { held_mods = 0; ++clear_count; }

static bool event(uint16_t keycode, uint8_t row, uint8_t col, bool pressed) {
    const matrix_row_t mask = (matrix_row_t)1 << col;
    if (pressed) matrix[row] |= mask;
    else matrix[row] &= ~mask;
    keyrecord_t record = {.event = {.key = {.row = row, .col = col}, .pressed = pressed}};
    const bool passed = process_record_user(keycode, &record);
    if (passed && keycode == MO(U_LOL_FN)) {
        const layer_state_t bit = (layer_state_t)1 << U_LOL_FN;
        layer_state = layer_state_set_user(pressed ? layer_state | bit : layer_state & ~bit);
    }
    return passed;
}

static void test_layout(void) {
    assert(U_BASE == 0 && U_NAV == 4 && U_MEDIA == 6 && U_FUN == 9);
    assert(U_LOL == 10 && U_LOL_FN == 11);
    assert(sizeof(league) / sizeof(*league) == 40);
    assert(media[4] == U_LOL_TOGGLE && media[23] == KC_F14);
    assert(normal_fn[4] == KC_PSCR);
    assert(league[14] == KC_LALT);
    assert(league[20] == KC_A && league[21] == KC_S && league[22] == KC_D);
    assert(league[23] == KC_F && league[24] == KC_B);
    assert(league[32] == MO(U_LOL_FN) && league[33] == KC_SPC && league[34] == KC_LCTL);
    assert(league_fn[0] == KC_ESC && league_fn[10] == KC_TAB && league_fn[4] == U_LOL_TOGGLE);
    const unsigned camera[] = {21, 22, 23, 11, 12};
    for (unsigned i = 0; i < 5; ++i) {
        assert(league_fn[camera[i]] == KC_F1 + i);
        assert(league_fn[camera[i]] == normal_fn[camera[i]]);
    }
    for (unsigned i = 0; i < 40; ++i) {
        assert(league[i] != KC_6 && league[i] != KC_7);
        assert(league_fn[i] != KC_6 && league_fn[i] != KC_7);
        if ((i < 30 && i % 10 >= 5) || (i >= 32 && i <= 37)) assert(league_fn[i] == KC_TRNS);
    }
}

static void test_roundtrip(bool initial_autoshift, bool thumb_released_first) {
    autoshift = initial_autoshift;
    caps_word = oneshot_mods = locked_mods = oneshot_layer = true;
    held_mods = 0xff;
    assert(event(LT(U_MEDIA, KC_ESC), 6, 11, true));
    layer_move(U_MEDIA);
    assert(event(KC_LCTL, 1, 2, true)); // Another key held across entry.
    assert(!event(U_LOL_TOGGLE, 4, 6, true));
    assert(layer_state == (1u << U_LOL));
    assert(!autoshift && !caps_word && !oneshot_mods && !locked_mods && !oneshot_layer && !held_mods);
    assert(hue == 30 && saturation == 255 && brightness == 32);
    // The old Media release cannot become Fn or emit Escape after switching.
    if (thumb_released_first) {
        assert(!event(LT(U_MEDIA, KC_ESC), 6, 11, false));
        assert(!event(U_LOL_TOGGLE, 4, 6, false));
    } else {
        assert(!event(U_LOL_TOGGLE, 4, 6, false));
        assert(!event(LT(U_MEDIA, KC_ESC), 6, 11, false));
    }
    assert(!event(KC_LCTL, 1, 2, false));
    assert(layer_state == (1u << U_LOL));
    assert(event(KC_1, 4, 10, true) && !autoshift);
    assert(event(KC_1, 4, 10, false));
    assert(event(MO(U_LOL_FN), 6, 11, true));
    assert(layer_state_is(U_LOL_FN) && hue == 30);
    assert(event(KC_TAB, 5, 10, true) && !autoshift);
    assert(event(KC_TAB, 5, 10, false));
    assert(event(MO(U_LOL_FN), 6, 11, false));
    assert(!layer_state_is(U_LOL_FN) && hue == 30);
    // Exit with Fn and another modifier held. Old releases must be swallowed.
    assert(event(MO(U_LOL_FN), 6, 11, true));
    assert(event(KC_LALT, 5, 6, true));
    held_mods = 4;
    assert(!event(U_LOL_TOGGLE, 4, 6, true));
    assert(layer_state == 0 && autoshift == initial_autoshift && !held_mods);
    assert(hue == 0 && saturation == 0 && brightness == 32);
    assert(!event(MO(U_LOL_FN), 6, 11, false));
    assert(!event(U_LOL_TOGGLE, 4, 6, false));
    assert(!event(KC_LALT, 5, 6, false));
    assert(layer_state == 0);
    assert(event(KC_Q, 4, 10, true));
    assert(event(KC_Q, 4, 10, false));
}

int main(void) {
    test_layout();
    // QMK initializes layer state before initializing its RGB driver.
    assert(layer_state_set_user(0) == 0);
    rgb_initialized = true;
    keyboard_post_init_user();
    assert(hue == 0 && saturation == 0 && brightness == 32);
    for (unsigned i = 0; i < 20; ++i) test_roundtrip(i % 2 == 0, i % 3 == 0);
    assert(clear_count == 40);
    puts("PASS: configured layouts, RGB startup ordering, colors, 20 mode roundtrips, held-key releases, Fn, and Auto Shift restoration");
}
