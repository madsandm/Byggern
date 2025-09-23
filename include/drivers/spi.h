#ifndef SPI_H
#define SPI_H

#define IO_BOARD_CS PB4

#include <stdint.h>

typedef struct {
    void (*init)();
    void (*transmit)(uint8_t data);
    char (*receive)();
    void (*transmit_receive)(char* data);
    void (*slave_select)(uint8_t port, uint8_t pin);
    void (*slave_deselect)(uint8_t port, uint8_t pin);
} ISPI;

ISPI spi;

#endif // SPI_H