#include "drivers/io.h"
#include "drivers/spi.h"


uint8_t io_readTouchpad(uint8_t index) {
    // Implement touchpad reading functionality here
    uint8_t arr[3];
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x01);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi_receive();
        _delay_us(2);
    }
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}

uint8_t io_readSlider(uint8_t index) {
    // Implement slider reading functionality here
    uint8_t arr[2];
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x02);
    _delay_us(40);
    for (int i = 0; i < 2; i++) {
        arr[i] = spi_receive();
        _delay_us(2);
    }
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}

uint8_t io_readJoystick(uint8_t index) {
    // Implement joystick reading functionality here
    uint8_t arr[3];
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x03);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi_receive();
        _delay_us(2);
    }
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}

void io_readJoystickBuffer(uint8_t* buffer) {
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x03);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        buffer[i] = spi_receive();
        _delay_us(2);
    }
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
}


uint8_t io_readButtons(uint8_t index) {
    // Implement button reading functionality here
    uint8_t arr[3];
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x04);
    _delay_us(40);
    for (int i = 0; i < 3; i++) {
        arr[i] = spi_receive();
        _delay_us(2);
    }
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
    return arr[index];  // Return the first byte as the touchpad value
}


void io_writeLeds(uint8_t index, uint8_t value) {
    // Implement LED writing functionality herePIOB = (1<<13)
    spi_slave_select(&PORTB, IO_BOARD_CS);
    spi_transmit(0x05);
    _delay_us(2);
    spi_transmit(index);
    _delay_us(2);
    spi_transmit(value);
    _delay_us(2);
    spi_slave_deselect(&PORTB, IO_BOARD_CS);
}
