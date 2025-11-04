#include "drivers/oled.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include "utilities.h"
#include <avr/io.h>
#include "fonts.h"
#include "drivers/sram.h"
#include <string.h>

void oled_init() {
    gpio_initPin(&DDRB, DISPLAY_CS, OUTPUT);
    gpio_initPin(&DDRB, DISPLAY_DC, OUTPUT);

    spi_slave_select(&PORTB, DISPLAY_CS);
    gpio_clearPin(&PORTB, DISPLAY_DC); // Command mode

    const char setup_commands[] = {
        0xae, // Display off
        0xa1, // Segment remap
        0xda, // Common pads hardware: alternative
        0x12,
        0xc8, // Common output scan direction: com63~com0
        0xa8, // Multiplex ration mode:63
        0x3f,
        0xd5, // Display divide ratio/osc. freq. mode
        0x80,
        0x81, // Contrast control
        0xff,
        0xd9, // Set pre-charge period
        0x21,
        0x20, // Set memory addressing mode
        0x02,
        0xdb, // VCOM deselect level mode
        0x30,
        0xad, // Master configuration
        0x00,
        0xa4, // Out follows RAM content
        0xa6, // Set normal display
        0xaf  // Display on
    };

    send_data(&spi_stream, setup_commands, sizeof(setup_commands));

    spi_slave_deselect(&PORTB, DISPLAY_CS);
    gpio_setPin(&PORTB, DISPLAY_DC); // Data mode
}

void oled_sram_init() {
    fb.front = FB1_BASE;
    fb.back = FB2_BASE;
    for (uint16_t i = 0; i < FRAMEBUFFER_SIZE; i++) {
        fb.back[i / 128][i % 128] = 0x00;
        fb.front[i / 128][i % 128] = 0x00;
    }
}

void oled_sram_flush() {
    spi_slave_select(&PORTB, DISPLAY_CS);
    for (uint8_t page = 0; page < 8; page++) {

        gpio_clearPin(&PORTB, DISPLAY_DC);  // Command mode
        spi_transmit(0xb0 + page);          // Set page address
        spi_transmit(0x00);
        spi_transmit(0x10);
        gpio_setPin(&PORTB, DISPLAY_DC);    // Data mode
        for (uint8_t seg = 0; seg < 128; seg++) {
            spi_transmit(fb.front[page][seg]);
        }
    }
    spi_slave_deselect(&PORTB, DISPLAY_CS);
}

void oled_sram_swap() {
    fb.front = (fb.front == FB1_BASE) ? FB2_BASE : FB1_BASE;
    fb.back = (fb.back == FB1_BASE) ? FB2_BASE : FB1_BASE;
}

void oled_present() {
    oled_sram_swap();
    memcpy(fb.back, fb.front, FRAMEBUFFER_SIZE);
    oled_sram_flush();
}

void oled_clear() {
    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t seg = 0; seg < 128; seg++) {
            fb.back[page][seg] = 0x00;
        }
    }
}

void oled_home() {
    fb.cursor_x = 0;
    fb.cursor_y = 0;
}

void oled_goto_line(uint8_t line) {
    if (line >= 8) return; // Out of bounds
    fb.cursor_x = 0;
    fb.cursor_y = line * 8;
}

void oled_clear_line(uint8_t line) {
    if (line >= 8) return; // Out of bounds
    for (uint8_t seg = 0; seg < 128; seg++) {
        fb.back[line][seg] = 0x00;
    }
}

void oled_pos(uint8_t row, uint8_t col) {
    if (row >= 64 || col >= 128) return; // Out of bounds
    if (row < 0 || col < 0) return;      // Out of bounds
    fb.cursor_x = col;
    fb.cursor_y = row;
}
    
void oled_print(const char* str) {
    while (*str) {
        if (*str < 32 || *str > 126) {
            str++;
            continue; // Skip unsupported characters
        }
        if (fb.cursor_x + 4 >= 128) { // Wrap to next line if needed
            fb.cursor_x = 0;
            fb.cursor_y += 8;
            if (fb.cursor_y >= 64) {
                fb.cursor_y = 0; // Wrap to top if needed
            }
        }
        uint8_t char_index = *str - 32;
        for (uint8_t i = 0; i < 4; i++) {
            fb.back[fb.cursor_y / 8][fb.cursor_x + i] = pgm_read_byte(&font4[char_index][i]);
        }
        fb.cursor_x += 4; // Move cursor forward
        str++;
    }
}

void oled_draw_pixel(uint8_t x, uint8_t y) {
    if (x >= 128 || y >= 64) return; // Out of bounds
    fb.back[y / 8][x] |= (1 << (y % 8));
}

void oled_draw_square(uint8_t x, uint8_t y, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            oled_draw_pixel(x + i, y + j);
        }
    }
}

void oled_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */
    while (1) {
        oled_draw_pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy+e_y < 0 */
    }
}

void oled_circle(int xm, int ym, int r) {
    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        oled_draw_pixel(xm - x, ym + y); /*   I. Quadrant */
        oled_draw_pixel(xm - y, ym - x); /*  II. Quadrant */
        oled_draw_pixel(xm + x, ym - y); /* III. Quadrant */
        oled_draw_pixel(xm + y, ym + x); /*  IV. Quadrant */
        r = err;
        if (r > x) err += ++x * 2 + 1;   /* e_xy+e_x > 0 */
        if (r <= y) err += ++y * 2 + 1;  /* e_xy+e_y < 0 */
    } while (x < 0);
}

void oled_erase_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    if (x1 >= 128 || y1 >= 64 || x2 >= 128 || y2 >= 64) return; // Out of bounds
    if (x1 > x2 || y1 > y2) return; // Invalid coordinates
    for (uint8_t x = x1; x <= x2; x++) {
        for (uint8_t y = y1; y <= y2; y++) {
            fb.back[y / 8][x] &= ~(1 << (y % 8));
        }
    }
}
