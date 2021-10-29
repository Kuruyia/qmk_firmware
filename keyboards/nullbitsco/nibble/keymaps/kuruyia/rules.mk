# Enable more features
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
RGBLIGHT_ENABLE = yes
RGBLIGHT_DRIVER = WS2812
WPM_ENABLE = yes

# Add user source files
SRC +=  nibble_oled_user.c \
        nibble_rgb_user.c
