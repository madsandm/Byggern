#include "uart.h"
#include "utilities.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

static int uart_transmit(const char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    return 0;
}

static int uart_transmit_with_stream(const char data, FILE *stream) {
    uart_transmit(data);
}

static void uart_init(const int baudrate, const bool enablePrintf) {
    // Set baud rate
    UBRR0H = (uint8_t)(baudrate >> 8);
    UBRR0L = (uint8_t)baudrate;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8data, 1stop bit
    UCSR0C = (1 << URSEL0) | (0 << USBS0) | (3 << UCSZ00);

    if (enablePrintf) {
        fdevopen(&uart_transmit_with_stream, NULL);
    }
}

static char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

static void uart_println(const char* str) {
    printf("%s\n\r", str);
}

IUART uart = {
    .init = uart_init,
    .println = uart_println,
    .write = uart_transmit,
    .read = uart_receive
};