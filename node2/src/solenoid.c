#include "solenoid.h"
#define SOLENOID_PIN PIO_PA16

void solenoid_init() {
    PIOA->PIO_PER = SOLENOID_PIN; // Enable PIOC pin 22 for output
    PIOA->PIO_OER = SOLENOID_PIN; // Set PIOC pin 22 as output
    PIOA->PIO_CODR = SOLENOID_PIN; // Set PIOC pin 22 low
}

void set_solenoid(uint8_t state) {
    if (state) {
        PIOA->PIO_SODR |= SOLENOID_PIN; // Set PIOC pin 22 high
    } else {
        PIOA->PIO_CODR |= SOLENOID_PIN; // Set PIOC pin 22 low
    }
}