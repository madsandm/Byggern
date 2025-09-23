#ifndef OLED_H
#define OLED_H


#define DISPLAY_CS PB0
#define DISPLAY_DC PB1

#include <stdint.h>

typedef struct {
    void (*init)();
    void (*clear)();
    void (*goto_line)(uint8_t line);
    void (*clear_line)(uint8_t line);
    void (*pos)(uint8_t row, uint8_t col);
    void (*print)(const char* str);
    void (*draw_pixel)(uint8_t x, uint8_t y);
    void (*draw_square)(uint8_t x, uint8_t y, uint8_t size);
    void (*line)(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void (*circle)(uint8_t xm, uint8_t ym, uint8_t radius);
} IOLED;

IOLED oled;

#endif