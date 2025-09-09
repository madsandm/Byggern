#ifndef UART_H
#define UART_H

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MY_UBRR FOSC/16/BAUD-1

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void (*init)(const int baudrate, const bool enablePrintf);
    void (*println)(const char* str);
    int (*write)(const char data);
    char (*read)();
} IUART;

IUART uart;

#endif // UART_H