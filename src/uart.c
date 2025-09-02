#include "uart.h"
#include <avr/io.h>
#include <stdint.h>

void uart_init(uint8_t baudrate) {
    // Set baud rate
    UBRR0H = (unsigned char)(baudrate >> 8);
    UBRR0L = (unsigned char)baudrate;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8data, 1stop bit
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

}

void uart_transmit(uint8_t data) {

}

uint8_t uart_receive(void) {

}