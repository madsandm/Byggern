#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "utilities.h"
#include "uart.h"
#include "sram.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();

    // Enable interrupts
    sei();

    test_latch();

    // SRAM_test();

    // initialize_pin(&DDRB, PB0, OUTPUT);
    // blinky(3);

    // while (1){
    //     uart_led_command();
    // }
    
    return 0;
}
