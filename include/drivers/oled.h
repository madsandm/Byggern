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
} IOLED;

IOLED oled;

#endif