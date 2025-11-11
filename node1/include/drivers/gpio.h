#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>

typedef enum PIN_MODE {
    INPUT = 0,
    OUTPUT = 1
} PIN_MODE;

void gpio_initPin(volatile uint8_t* port, uint8_t pin, PIN_MODE mode);
void gpio_togglePin(volatile uint8_t* port, uint8_t pin);
void gpio_setPin(volatile uint8_t* port, uint8_t pin);
void gpio_clearPin(volatile uint8_t* port, uint8_t pin);
uint8_t gpio_readPin(volatile uint8_t* port, uint8_t pin);

#endif // GPIO_H