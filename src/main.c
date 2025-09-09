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

    // test_latch();

    while (true) {
        SRAM_test();
        break;
    }

    // initialize_pin(&DDRB, PB0, OUTPUT);
    // blinky(3);

    // while (1){
    //     uart_led_command();
    // }

    while (true);
    
    return 0;
}
