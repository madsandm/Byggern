#include "solenoid.h"
#define SOLENOID_PIN PIO_PC22

void solenoid_init() {
    PIOC->PIO_PER = SOLENOID_PIN; // Enable PIOC pin 22 for output
    PIOC->PIO_OER = SOLENOID_PIN; // Set PIOC pin 22 as output
    PIOC->PIO_CODR = SOLENOID_PIN; // Set PIOC pin 22 low
}

void set_solenoid(uint8_t state) {
    if (state) {
        PIOC->PIO_SODR = SOLENOID_PIN; // Set PIOC pin 22 high
    } else {
        PIOC->PIO_CODR = SOLENOID_PIN; // Set PIOC pin 22 low
    }
}