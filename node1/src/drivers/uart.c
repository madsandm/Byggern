#include "drivers/uart.h"
#include "utilities.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

static char rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_head = 0;
static uint8_t rx_tail = 0;

int uart_write(const char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    return 0;
}

int uart_putchar(const char data, FILE *stream) {
    uart_write(data);
    return 0;
}

/**
 * Initializes the UART with the specified baud rate.
 * If enablePrintf is true, it sets up the UART for use with printf.
 */
void uart_init(const int ubbr, const bool enablePrintf) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubbr >> 8);
    UBRR0L = (uint8_t)ubbr;

    // Enable receiver and transmitter and RX interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format: 8data, 1stop bit
    UCSR0C = (1 << URSEL0) | (0 << USBS0) | (3 << UCSZ00);

    if (enablePrintf) {
        fdevopen(&uart_putchar, NULL);
    }
}

static char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

ISR(USART0_RXC_vect) {
    char data = uart_receive();
    // Store received data in buffer
    rx_buffer[rx_head] = data;
    rx_head = (rx_head + 1) % RX_BUFFER_SIZE;
}

bool uart_available() {
    return rx_head != rx_tail;
}

char uart_read() {
    if (!uart_available()) {
        printf("ERROR: No data available while trying to read UART\n");
        return 0; // No data available
    }

    char data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return data;
}

void uart_println(const char* str) {
    printf("%s\n", str);
}

//FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

