#include "drivers/oled.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include "utilities.h"
#include <avr/io.h>
#include "fonts.h"

static void oled_init() {
    GPIO.initPin(&DDRB, DISPLAY_CS, OUTPUT);
    GPIO.initPin(&DDRB, DISPLAY_DC, OUTPUT);

    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode

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

    send_data(spi.stream, setup_commands, sizeof(setup_commands));

    spi.slave_deselect(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
}

static void oled_clear() {
    spi.slave_select(&PORTB, DISPLAY_CS);
    for (uint8_t page = 0; page < 8; page++) {

        GPIO.clearPin(&PORTB, DISPLAY_DC);  // Command mode
        spi.transmit(0xb0 + page);          // Set page address
        // spi.transmit(0x0 + (seg & 0x0f));   // Set lower column address
        // spi.transmit(0x10 + (seg >> 4));    // Set higher column address
        spi.transmit(0x00);
        spi.transmit(0x10);
        GPIO.setPin(&PORTB, DISPLAY_DC);    // Data mode

        for (uint8_t seg = 0; seg < 128; seg++) {
            spi.transmit(0);
        }
    }
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_home() {
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xb0);
    spi.transmit(0x0);
    spi.transmit(0x10);
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_goto_line(uint8_t line) {
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xb0 + line);
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_clear_line(uint8_t line) {
    if (line >= 8) return; // Out of bounds

    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xb0 + line); // Set page address
    spi.transmit(0x0);         // Set lower column address
    spi.transmit(0x10);        // Set higher column address

    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode

    for (uint8_t seg = 0; seg < 128; seg++) {
        spi.transmit(0x00);
    }
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_pos(uint8_t row, uint8_t col) {
    if (row >= 64 || col >= 128) return; // Out of bounds

    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC);   // Command mode
    spi.transmit(0xb0 + (row / 8));      // Set page address
    spi.transmit(0x00 + (col & 0x0f));   // Set lower column
    spi.transmit(0x10 + (col >> 4));     // Set higher column address
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_print(char* str) {
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode

    while (*str) {
        char c = *str++;
        if (c < 0x20 || c > 0x7E) c = '?'; // Replace unsupported characters
        for (uint8_t i = 0; i < 4; i++) {
            spi.transmit(pgm_read_byte(&font4[c - 0x20][i]));
        }
        spi.transmit(0x00); // Space between characters
}

    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_draw_pixel(uint8_t x, uint8_t y) {
    if (x >= 128 || y >= 64) return; // Out of bounds

    oled.pos(y, x);
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
    spi.transmit(1 << (y % 8));
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

static void oled_draw_square(uint8_t x, uint8_t y, uint8_t size) {
    if (x >= 128 || y >= 64 || size == 0 || size > 8) return; // Out of bounds or invalid size
    //if (x + size > 128) size = 128 - x; // Adjust size if it exceeds display width
    //if (y + size > 64) size = 64 - y;   // Adjust size if it exceeds display height

    oled.pos(y, x);
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
    for (uint8_t i = 0; i < size; i++) {
        spi.transmit(pgm_read_byte(&squares[size - 1][i]));
    }
    spi.slave_deselect(&PORTB, DISPLAY_CS); 
}

static void oled_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
        // Set pixel (x1, y1)
        oled_pos(y1 / 8, x1);
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(1 << (y1 % 8));
        spi.slave_deselect(&PORTB, DISPLAY_CS);
        if (x1==x2 && y1==y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx;
        }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

static void oled_circle(int xm, int ym, int r) {
    int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
    do {
        oled_pos((ym + y) / 8, xm - x);
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(1 << ((ym + y) % 8));
        spi.slave_deselect(&PORTB, DISPLAY_CS);
        oled_pos((ym - y) / 8, xm + x);
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(1 << ((ym - y) % 8));
        spi.slave_deselect(&PORTB, DISPLAY_CS);
        oled_pos((ym + x) / 8, xm + y);
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(1 << ((ym + x) % 8));
        spi.slave_deselect(&PORTB, DISPLAY_CS);
        oled_pos((ym - x) / 8, xm - y);
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(1 << ((ym - x) % 8));
        spi.slave_deselect(&PORTB, DISPLAY_CS);
        r = err;
        if (r > x) err += ++x*2+1;   /* err_x */
        if (r <= y) err += ++y*2+1; /* err_y */
    } while (x < 0);
}


IOLED oled = {
    .init = oled_init,
    .clear = oled_clear,
    .goto_line = oled_goto_line,
    .clear_line = oled_clear_line,
    .pos = oled_pos,
    .print = oled_print,
    .draw_pixel = oled_draw_pixel,
    .draw_square = oled_draw_square,
    .line = oled_line,
    .circle = oled_circle
};