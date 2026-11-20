// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

// Personal Cheapino QWERTY/VI layout. Keep generated Babel files untouched.
#if defined(KEYBOARD_cheapino) && defined(MIRYOKU_NAV_VI) && !defined(MIRYOKU_LAYERS_FLIP)
#define MIRYOKU_LEAGUE
#define MIRYOKU_LAYER_LIST \
MIRYOKU_X(BASE, "Base") \
MIRYOKU_X(EXTRA, "Extra") \
MIRYOKU_X(TAP, "Tap") \
MIRYOKU_X(BUTTON, "Button") \
MIRYOKU_X(NAV, "Nav") \
MIRYOKU_X(MOUSE, "Mouse") \
MIRYOKU_X(MEDIA, "Media") \
MIRYOKU_X(NUM, "Num") \
MIRYOKU_X(SYM, "Sym") \
MIRYOKU_X(FUN, "Fun") \
MIRYOKU_X(LOL, "League") \
MIRYOKU_X(LOL_FN, "League Fn")

#define MIRYOKU_LAYERMAPPING_LOL MIRYOKU_MAPPING
#define MIRYOKU_LAYERMAPPING_LOL_FN MIRYOKU_MAPPING

#define MIRYOKU_LAYER_LOL \
KC_1,             KC_2,             KC_3,             KC_4,             KC_5,             KC_Y,             KC_U,             KC_I,             KC_O,             KC_P,             \
KC_Q,             KC_W,             KC_E,             KC_R,             KC_LALT,          KC_H,             KC_J,             KC_K,             KC_L,             KC_QUOT,          \
KC_A,             KC_S,             KC_D,             KC_F,             KC_B,             KC_N,             KC_M,             KC_COMM,          KC_DOT,           KC_SLSH,          \
U_NP,             U_NP,             MO(U_LOL_FN),     KC_SPC,           KC_LCTL,          KC_ENT,           KC_BSPC,          KC_LSFT,          U_NP,             U_NP

#define MIRYOKU_LAYER_LOL_FN \
KC_ESC,           KC_NO,            KC_NO,            KC_NO,            U_LOL_TOGGLE,     KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          \
KC_TAB,           KC_F4,            KC_F5,            KC_NO,            KC_NO,            KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          \
KC_NO,            KC_F1,            KC_F2,            KC_F3,            KC_NO,            KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          \
U_NP,             U_NP,             KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          U_NP,             U_NP

#define U_MEDIA_T U_LOL_TOGGLE
#else
#define U_MEDIA_T U_NA
#endif

// QWERTY + VI: Media + V sends F14 for a host-side dictation shortcut.
// Replaces the Media layer lock at that position.
#if defined(MIRYOKU_NAV_VI)
#define MIRYOKU_LAYER_MEDIA \
TD(U_TD_BOOT),     TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_MEDIA_T,         RGB_MOD,           RGB_HUI,           RGB_SAI,           RGB_VAI,           RGB_TOG,           \
KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              KC_MPRV,           KC_VOLD,           KC_VOLU,           KC_MNXT,           U_NU,              \
U_NA,              KC_ALGR,           TD(U_TD_U_FUN),    KC_F14,            U_NA,              U_NU,              U_NU,              U_NU,              U_NU,              OU_AUTO,           \
U_NP,              U_NP,              U_NA,              U_NA,              U_NA,              KC_MSTP,           KC_MPLY,           KC_MUTE,           U_NP,              U_NP
#endif

