#ifndef UART_H
#define UART_H

//#define FOSC 4915200 // Clock Speed
#define FOSC F_CPU
#define BAUD 19200
#define MY_UBRR FOSC/16/BAUD-1
#define RX_BUFFER_SIZE 32

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void (*init)(const int UBRR, const bool enablePrintf);
    void (*println)(const char* str);
    int (*write)(const char data);
    char (*read)();
    bool (*available)();
} IUART;

IUART uart;

#endif // UART_H