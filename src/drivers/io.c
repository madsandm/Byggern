#include "drivers/io.h"
#include "drivers/spi.h"


static uint8_t read_touchpad(uint8_t index) {
    // Implement touchpad reading functionality here
    uint8_t arr[3];
    spi.slave_select(&PORTB, IO_BOARD_CS);
    spi.transmit(0x01);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi.receive();
        _delay_us(2);
    }
    spi.slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}

static uint8_t read_slider(uint8_t index) {
    // Implement slider reading functionality here
    uint8_t arr[2];
    spi.slave_select(&PORTB, IO_BOARD_CS);
    spi.transmit(0x02);
    _delay_us(40);
    for (int i = 0; i < 2; i++) {
        arr[i] = spi.receive();
        _delay_us(2);
    }
    spi.slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}

static uint8_t read_joystick(uint8_t index) {
    // Implement joystick reading functionality here
    uint8_t arr[3];
    spi.slave_select(&PORTB, IO_BOARD_CS);
    spi.transmit(0x03);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi.receive();
        _delay_us(2);
    }
    spi.slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}


static uint8_t read_buttons(uint8_t index) {
    // Implement button reading functionality here
    uint8_t arr[3];
    spi.slave_select(&PORTB, IO_BOARD_CS);
    spi.transmit(0x04);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi.receive();
        _delay_us(2);
    }
    spi.slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}


static void write_leds(uint8_t index, uint8_t value) {
    // Implement LED writing functionality here
    spi.slave_select(&PORTB, IO_BOARD_CS);
    spi.transmit(0x05);
    _delay_us(2);
    spi.transmit(index);
    _delay_us(2);
    spi.transmit(value);
    _delay_us(2);
    spi.slave_deselect(&PORTB, IO_BOARD_CS);
}


IIO io = {
    .read_touchpad = read_touchpad,
    .read_joystick = read_joystick,
    .read_slider = read_slider,
    .read_buttons = read_buttons,
    .write_leds = write_leds
};