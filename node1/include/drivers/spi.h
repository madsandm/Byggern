#ifndef SPI_H
#define SPI_H

#define IO_BOARD_CS PB4

#include <stdio.h>
#include <stdint.h>

void    spi_init();
void    spi_transmit(uint8_t data);
void    spi_transmit_multi(const uint8_t* data, uint8_t size);
uint8_t spi_receive();
void    spi_receive_multi_into(uint8_t* dst, uint8_t size);
void    spi_transmit_receive(uint8_t* data);
void    spi_slave_select(volatile uint8_t* port, uint8_t pin);
void    spi_slave_deselect(volatile uint8_t* port, uint8_t pin);
FILE    spi_stream;

#endif // SPI_H
