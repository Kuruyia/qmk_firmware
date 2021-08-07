#include <stddef.h>

#include QMK_KEYBOARD_H

#include "nibble_oled_user.h"

#define PROGRESSBAR_BORDER 0xFF
#define PROGRESSBAR_EMPTY  0x81
#define PROGRESSBAR_FULL   0x99

static void oled_write_raw_byte_at(const char data, const uint8_t x, const uint8_t y)
{
    oled_write_raw_byte(data, OLED_DISPLAY_HEIGHT * y + x);
}

static void draw_graphic_P(const char* buffer, const size_t bufferLen, const uint8_t x, const uint8_t y)
{
    for (size_t i = 0; i < bufferLen; ++i)
    {
        oled_write_raw_byte_at(pgm_read_byte(buffer + i), x + i, y);
    }
}

void draw_circle(const uint8_t x, const uint8_t y)
{
    // Circle outline graphic
    static const char PROGMEM circle_outline[] = {0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C};

    draw_graphic_P(circle_outline, sizeof(circle_outline), x, y);
}

void draw_filled_circle(const uint8_t x, const uint8_t y)
{
    // Filled circle graphic
    static const char PROGMEM circle_filled[] = {0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C};

    draw_graphic_P(circle_filled, sizeof(circle_filled), x, y);
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
