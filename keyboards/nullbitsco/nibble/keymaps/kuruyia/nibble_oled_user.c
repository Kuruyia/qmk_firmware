#include <stddef.h>

#include QMK_KEYBOARD_H

#include "nibble_oled_user.h"

#define PROGRESSBAR_BORDER 0xFF
#define PROGRESSBAR_EMPTY  0x81
#define PROGRESSBAR_FULL   0x99

void oled_write_raw_byte_at(const char data, const uint8_t x, const uint8_t y)
{
    oled_write_raw_byte(data, OLED_DISPLAY_HEIGHT * y + x);
}

void draw_progress_bar(const uint8_t progress, const uint8_t y)
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
