#ifndef UART_H
#define UART_H

#define FOSC 4915200UL // Clock Speed
#define BAUD 9600
#define UBBR FOSC/(16*BAUD)-1

#include <stdint.h>

void uart_init(uint16_t baudrate);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);

#endif // UART_H