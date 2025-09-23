#include "drivers/oled.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <avr/io.h>

static void oled_init() {
    GPIO.initPin(&DDRB, DISPLAY_CS, OUTPUT);
    GPIO.initPin(&DDRB, DISPLAY_DC, OUTPUT);
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xae);
    spi.transmit(0xa1);
    spi.transmit(0xda);
    spi.transmit(0x12);
    spi.transmit(0xc8);
    spi.transmit(0xa8);
    spi.transmit(0x3f);
    spi.transmit(0xd5);
    spi.transmit(0x80);
    spi.transmit(0x81);
    spi.transmit(0x50);
    spi.transmit(0xd9);
    spi.transmit(0x21);
    spi.transmit(0x20);
    spi.transmit(0x02);
    spi.transmit(0xdb);
    spi.transmit(0x30);
    spi.transmit(0xdb);
    spi.transmit(0x30);
    spi.transmit(0xad);
    spi.transmit(0x00);
    spi.transmit(0xa4);
    spi.transmit(0xa6);
    spi.transmit(0xaf);
    spi.slave_deselect(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
}

static void oled_clear() {
    for (uint8_t page = 0; page < 8; page++) {
        spi.slave_select(&PORTB, DISPLAY_CS);
        GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
        spi.transmit(0xb0 + page); // Set page address
        GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
        spi.slave_deselect(&PORTB, DISPLAY_CS);
    }
}

static void oled_draw_pixel(uint8_t x, uint8_t y, int on) {
    if (x >= 128 || y >= 64) return; // Out of bounds

    uint8_t page = y / 8;
    uint8_t bit_position = y % 8;

    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xb0 + page); // Set page
    spi.transmit(0x00 + (x & 0x0f)); // Set lower column address
    spi.transmit(0x10 + ((x >> 4) & 0x0f)); // Set higher column address

    GPIO.setPin(&PORTB, DISPLAY_DC); // Data mode
    spi.transmit(on ? 0xff : 0x00); // Set pixel
    GPIO.clearPin(&PORTB, DISPLAY_DC); // Command mode
    spi.slave_deselect(&PORTB, DISPLAY_CS);
}

IOLED oled = {
    .init = oled_init,
    .clear = oled_clear,
    .draw_pixel = oled_draw_pixel
};