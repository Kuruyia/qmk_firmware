#include QMK_KEYBOARD_H

#include "nibble_rgb_user.h"

// RGB swirl mode settings
const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {50, 100, 100};

// RGB functions
void rgb_previous_anim_user(void) {
    switch (rgblight_get_mode()) {
        case RGBLIGHT_MODE_RAINBOW_SWIRL:
            rgblight_mode(RGBLIGHT_MODE_BREATHING);
            break;

        case RGBLIGHT_MODE_BREATHING:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
            break;

        default:
            break;
    }
}

void rgb_next_anim_user(void) {
    switch (rgblight_get_mode()) {
        case RGBLIGHT_MODE_BREATHING:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
            break;

        case RGBLIGHT_MODE_RAINBOW_SWIRL:
            rgblight_mode(RGBLIGHT_MODE_BREATHING);
            break;

        default:
            break;
    }
}
