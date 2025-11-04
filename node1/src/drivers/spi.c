#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

static void spi_init() {
    DDRB |= (1 << DDB5) | (1 << DDB7); // Set SCK and MOSI as output;
    gpio_initPin(&DDRB, DDB4, OUTPUT); // Set SS1 as output before enabling SPI
    gpio_initPin(&DDRB, DDB1, OUTPUT); // Set SS2 as output

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, set clock rate fck/16
}

static void spi_transmit(char data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

static void spi_transmit_multi(const char* data, unsigned char size) {
    for(char i = 0; i < size; i++) {
        spi_transmit(data[i]);
    }
}

static int spi_putchar(const char data, FILE* stream) {
    spi_transmit(data);
    return 0;
}

static char spi_receive() {
    SPDR = 0xFF; // Send dummy byte to generate clock
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

static char* spi_receive_multi(unsigned char size) {
    char* result = malloc(size);
    if (result == NULL) {
        // TODO: Handle
    }

    for (char i = 0; i < size; i++) {
        result[i] = spi_receive();
    }

    return result;
}

static void spi_transmit_receive(char* data) {
    SPDR = *data;
    while (!(SPSR & (1 << SPIF)));
    *data = SPDR;
}

static void spi_slave_select(volatile uint8_t* port, uint8_t pin) {
    gpio_clearPin(port, pin);
}

static void spi_slave_deselect(volatile uint8_t* port, uint8_t pin) {
    gpio_setPin(port, pin);
}

FILE spi_stream = FDEV_SETUP_STREAM(spi_putchar, NULL, _FDEV_SETUP_WRITE);

ISPI spi = {
    .init = spi_init,
    .transmit = spi_transmit,
    .transmit_multi = spi_transmit_multi,
    .receive = spi_receive,
    .receive_multi = spi_receive_multi,
    .transmit_receive = spi_transmit_receive,
    .slave_select = spi_slave_select,
    .slave_deselect = spi_slave_deselect,
    .stream = &spi_stream
};
