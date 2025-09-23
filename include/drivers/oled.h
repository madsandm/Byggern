#ifndef OLED_H
#define OLED_H


#define DISPLAY_CS PB0
#define DISPLAY_DC PB1

#include <stdint.h>

typedef struct {
    void (*init)();
    void (*clear)();
    void (*draw_pixel)(uint8_t x, uint8_t y, int on);
} IOLED;

IOLED oled;

#endif