#include "blinky.h"
#include <avr/io.h>

void blinky(){
    // Implement LED blinking functionality here
    while(1){
        PORTB ^= (1 << PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 100000; i++); // Simple delay
    }
}
