#ifndef SPI_H
#define SPI_H

#define IO_BOARD_CS PB4

#include <stdio.h>
#include <stdint.h>

typedef struct {
    void (*init)();
    void (*transmit)(char data);
    void (*transmit_multi)(const char* data, unsigned char size);
    char (*receive)();
    char* (*receive_multi)(unsigned char size);
    void (*transmit_receive)(char* data);
    void (*slave_select)(volatile uint8_t* port, uint8_t pin);
    void (*slave_deselect)(volatile uint8_t* port, uint8_t pin);
    FILE* stream;
} ISPI;

ISPI spi;

#endif // SPI_H
