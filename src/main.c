#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "utilities.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/gpio.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();

    // Enable interrupts
    sei();

    // test_latch();

    GPIO.initPin(&DDRB, PB0, OUTPUT);
    blinky(3);

    sram.test();

    // while (1){
    //     uart_led_command();
    // }

    while (true);
    
    return 0;
}
