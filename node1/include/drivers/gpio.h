#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>

typedef enum PIN_MODE {
    INPUT = 0,
    OUTPUT = 1
} PIN_MODE;

typedef struct {
    void (*initPin)(volatile uint8_t* port, uint8_t pin, PIN_MODE mode);
    void (*togglePin)(volatile uint8_t* port, uint8_t pin);
    void (*setPin)(volatile uint8_t* port, uint8_t pin);
    void (*clearPin)(volatile uint8_t* port, uint8_t pin);
} IGPIO;

IGPIO GPIO;

#endif // GPIO_H