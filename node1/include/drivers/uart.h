#ifndef UART_H
#define UART_H

//#define FOSC 4915200 // Clock Speed
#define FOSC F_CPU
#define BAUD 19200
#define MY_UBRR FOSC/16/BAUD-1
#define RX_BUFFER_SIZE 4

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void uart_init(const int UBRR, const bool enablePrintf);
void uart_println(const char* str);
int uart_write(const char data);
char uart_read();
bool uart_available();

#endif // UART_H
