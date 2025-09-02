#ifndef UART_H
#define UART_H

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MY_UBRR FOSC/16/BAUD-1

#include <stdint.h>

void uart_init(int baudrate);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);

#endif // UART_H