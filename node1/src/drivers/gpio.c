#include "drivers/gpio.h"

void gpio_initPin(volatile uint8_t* port, uint8_t pin, PIN_MODE mode) {
    *port |= (mode << pin);
}

void gpio_togglePin(volatile uint8_t* port, uint8_t pin) {
    // Toggle the specified pin on the specified port
    *port ^= (1 << pin);
}

void gpio_setPin(volatile uint8_t* port, uint8_t pin) {
    *port |= (1 << pin);
}

void gpio_clearPin(volatile uint8_t* port, uint8_t pin) {
    *port &= ~(1 << pin);
}
