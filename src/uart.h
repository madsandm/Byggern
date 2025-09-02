#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint8_t baudrate);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);

#endif // UART_H