#include "uart.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_BUFFER_LENGTH 32
uint8_t UART_BUFFER[UART_BUFFER_LENGTH];
uint8_t uartLastRead = 0;
uint8_t uartLastRecevied = 0;

void uart_init(int baudrate) {
    // Set baud rate
    baudrate = FOSC / (16 * baudrate) - 1;
    UBRR0H = (uint8_t)(baudrate >> 8);
    UBRR0L = (uint8_t)baudrate;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8data, 1stop bit
    UCSR0C = (1 << URSEL0) | (3 << UCSZ00);

}

void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
