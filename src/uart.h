#ifndef UART_H
#define UART_H

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MY_UBRR FOSC/16/BAUD-1

#include <stdint.h>

void uart_init(int baudrate);
void uart_transmit(uint8_t data);
char uart_receive(void);
void uart_printf_enable();
void uart_printline(const char* str);

#endif // UART_H