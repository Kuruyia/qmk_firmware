/* Copyright 2020 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "nibble_rgb_user.h"

#define PROGRESSBAR_BORDER 0xFF
#define PROGRESSBAR_EMPTY  0x81
#define PROGRESSBAR_FULL   0x99

enum layer_names { _MA, _FN };

enum custom_keycodes {
    KC_NRGB = SAFE_RANGE,
    KC_PRGB,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MA] = LAYOUT_iso(
             KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
    KC_MUTE, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_DEL,
    KC_MPLY, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_PGUP,
    KC_F15,  KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    KC_F16,  KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                    KC_RALT, MO(_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [_FN] = LAYOUT_iso(
               RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_HOME,  KC_END,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           KC_INS,
    KC_MNXT, KC_NLCK, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAI, _______,
    _______, _______, _______, _______,                            _______,                   _______, _______, RGB_TOG, KC_PRGB, RGB_VAD, KC_NRGB
  ),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Send keystrokes to host keyboard, if connected (see readme)
    process_record_remote_kb(keycode, record);

    switch (keycode) {
        case KC_PRGB:
            if (record->event.pressed) {
                rgb_previous_anim_user();
            }

            break;

        case KC_NRGB:
            if (record->event.pressed) {
                rgb_next_anim_user();
            }

            break;
    }

    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (layer_state_is(1)) {
        // FN layer, change brightness
        if (clockwise) {
            tap_code(KC_BRIU);
        } else {
            tap_code(KC_BRID);
        }
    } else {
        // Normal layer, change volume
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }

    return true;
}

void matrix_init_user(void) {
    // Initialize remote keyboard, if connected (see readme)
    matrix_init_remote_kb();
}

void matrix_scan_user(void) {
    // Scan and parse keystrokes from remote keyboard, if connected (see readme)
    matrix_scan_remote_kb();
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

static inline void oled_write_raw_byte_at(const char data, const uint8_t x, const uint8_t y)
{
    oled_write_raw_byte(data, OLED_DISPLAY_HEIGHT * y + x);
}

static void draw_progress_bar(const uint8_t progress, const uint8_t y)
{
    // Draw the static parts
    oled_write_raw_byte_at(PROGRESSBAR_BORDER, 0, y);
    oled_write_raw_byte_at(PROGRESSBAR_EMPTY, 1, y);
    oled_write_raw_byte_at(PROGRESSBAR_EMPTY, 2, y);

    oled_write_raw_byte_at(PROGRESSBAR_EMPTY, OLED_DISPLAY_HEIGHT - 3, y);
    oled_write_raw_byte_at(PROGRESSBAR_EMPTY, OLED_DISPLAY_HEIGHT - 2, y);
    oled_write_raw_byte_at(PROGRESSBAR_BORDER, OLED_DISPLAY_HEIGHT - 1, y);

    // Draw the bar
    size_t i = 3;
    const uint8_t endCol = progress * (OLED_DISPLAY_HEIGHT - 6) / UINT8_MAX + 3;

    for (; i < endCol; ++i)
    {
        oled_write_raw_byte_at(PROGRESSBAR_FULL, i, y);
    }

    for (; i < OLED_DISPLAY_HEIGHT - 3; ++i)
    {
        oled_write_raw_byte_at(PROGRESSBAR_EMPTY, i, y);
    }
}

void oled_task_user(void) {
    // Get the current WPM
    char wpmBuffer[4];
    itoa(get_current_wpm(), wpmBuffer, 10);

    // Show the WPM
    oled_write_ln_P(PSTR("WPM"), false);
    oled_write_ln(wpmBuffer, false);
    oled_advance_page(false);

    // Show the current layer
    oled_write_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state)) {
        case _MA:
            oled_write_ln_P(PSTR("Main"), false);
            break;
        case _FN:
            oled_write_ln_P(PSTR("FN"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Show RGB state
    oled_set_cursor(0, 6);
    oled_write_ln_P(PSTR("RGB"), false);

    draw_progress_bar(rgblight_get_val(), 7);

    // Show LED state
    led_t led_state = host_keyboard_led_state();
    oled_set_cursor(0, 15);

    oled_write_P(PSTR("C"), led_state.caps_lock);
    oled_advance_char();

    oled_write_P(PSTR("N"), led_state.num_lock);
    oled_advance_char();

    oled_write_P(PSTR("S"), led_state.scroll_lock);
    oled_advance_char();
}
#endif
