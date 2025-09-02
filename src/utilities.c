#include "utilities.h"
#include <avr/io.h>


void initializePin(uint8_t* DD, uint8_t pin, uint8_t mode) {
    *DD |= (mode << pin);
}

void togglePin(uint8_t* port, uint8_t pin) {
    // Toggle the specified pin on the specified port
    *port ^= (1 << pin);
}

void blinky(){
    // Implement LED blinking functionality here
    initializePin(&DDRB, PB0, OUTPUT);
    while(1){
        togglePin(&PORTB, PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 100000; i++); // Simple delay
        
    }
}
