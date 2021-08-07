#ifndef NIBBLE_OLED_USER_H
#define NIBBLE_OLED_USER_H

void oled_write_raw_byte_at(const char data, const uint8_t x, const uint8_t y);
void draw_progress_bar(const uint8_t progress, const uint8_t y);

#endif // NIBBLE_OLED_USER_H
