#ifndef OLED_H
#define OLED_H

#define DISPLAY_CS PB0
#define DISPLAY_DC PB1

#define FRAMEBUFFER_SIZE 1024

#include <stdint.h>
#include <stdlib.h>
#include "drivers/sram.h"

void oled_init();
void oled_clear();
void oled_goto_line(uint8_t line);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row, uint8_t col);
void oled_print(const char* str);
void oled_draw_pixel(uint8_t x, uint8_t y);
void oled_draw_square(uint8_t x, uint8_t y, uint8_t size);
void oled_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void oled_circle(int xm, int ym, int radius);
void oled_erase_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void oled_sram_init();
void oled_sram_flush();
void oled_sram_swap();
void oled_present();

typedef uint8_t (*FramePtr)[128];
#define FB1_BASE ((FramePtr)EXTERNAL_RAM_ADDRESS)
#define FB2_BASE ((FramePtr)(EXTERNAL_RAM_ADDRESS + FRAMEBUFFER_SIZE))

typedef struct {
    FramePtr front;
    FramePtr back;
    uint8_t cursor_x;
    uint8_t cursor_y;
} Buffers;

static Buffers fb;

#endif // OLED_H