#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "utilities.h"
#include "drivers/uart.h"

int main() {
    uart.init(MY_UBRR, true);

    // Enable interrupts
    sei();

    initialize_pin(&DDRB, PB0, OUTPUT);
    blinky(3);

    while (1){
        uart_led_command();
    }
    
    return 0;
}
