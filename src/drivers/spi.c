#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <avr/io.h>

static void spi_init() {
    DDRB = (1 << DDB5) | (1 << DDB7); // Set SCK and MOSI as output;
    //GPIO.initPin(&DDRB, DDB5, OUTPUT); // Set MOSI as output
    //GPIO.initPin(&DDRB, DDB7, OUTPUT); // Set SCK as output
    
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, set clock rate fck/16

    GPIO.initPin(&DDRB, DDB4, OUTPUT); // Set SS as output
    // Init second slave select pin when needed
}

static void spi_transmit(char data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

static char spi_receive() {
    SPDR = 0xFF; // Send dummy byte to generate clock
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

static void spi_transmit_receive(char* data) {
    SPDR = *data;
    while (!(SPSR & (1 << SPIF)));
    *data = SPDR;
}

static void spi_slave_select(uint8_t* port, uint8_t pin) {
    GPIO.clearPin(port, pin);
}

static void spi_slave_deselect(uint8_t* port, uint8_t pin) {
    GPIO.setPin(port, pin);
}

ISPI spi = {
    .init = spi_init,
    .transmit = spi_transmit,
    .receive = spi_receive,
    .transmit_receive = spi_transmit_receive,
    .slave_select = spi_slave_select,
    .slave_deselect = spi_slave_deselect
};