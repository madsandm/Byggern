#include "drivers/gpio.h"

static void initPin(volatile uint8_t* port, uint8_t pin, PIN_MODE mode) {
    *port |= (mode << pin);
}

static void togglePin(volatile uint8_t* port, uint8_t pin) {
    // Toggle the specified pin on the specified port
    *port ^= (1 << pin);
}

static void setPin(volatile uint8_t* port, uint8_t pin) {
    *port |= (1 << pin);
}

static void clearPin(volatile uint8_t* port, uint8_t pin) {
    *port &= ~(1 << pin);
}

IGPIO GPIO = {
    .initPin = initPin,
    .togglePin = togglePin,
    .setPin = setPin,
    .clearPin = clearPin
};