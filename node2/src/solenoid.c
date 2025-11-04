#include "solenoid.h"
#define SOLENOID_PIN PIO_PD7

void solenoid_init() {
    PIOD->PIO_PER = SOLENOID_PIN; // Enable PIOC pin 22 for output
    PIOD->PIO_OER = SOLENOID_PIN; // Set PIOC pin 22 as output
    PIOD->PIO_CODR = SOLENOID_PIN; // Set PIOC pin 22 low
}

void set_solenoid(uint8_t state) {
    if (state) {
        PIOD->PIO_SODR |= SOLENOID_PIN; // Set PIOC pin 22 high
    } else {
        PIOD->PIO_CODR |= SOLENOID_PIN; // Set PIOC pin 22 low
    }
}