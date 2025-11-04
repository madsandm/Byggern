#ifndef SPI_H
#define SPI_H

#define IO_BOARD_CS PB4

#include <stdio.h>
#include <stdint.h>

void spi_init();
void spi_transmit(char data);
void spi_transmit_multi(const char* data, unsigned char size);
char spi_receive();
char* spi_receive_multi(unsigned char size);
void spi_transmit_receive(char* data);
void spi_slave_select(volatile uint8_t* port, uint8_t pin);
void spi_slave_deselect(volatile uint8_t* port, uint8_t pin);
FILE spi_stream;

#endif // SPI_H
