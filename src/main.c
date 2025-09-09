#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "utilities.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();

    // Enable interrupts
    sei();

    // test_latch();

    GPIO.initPin(&DDRB, PB0, OUTPUT);
    while (true) {
        GPIO.togglePin(&PORTB, PB0); // Toggle LED connected to PB0
        delay(500);
    }

    // sram.test();

    // while (1){
    //     uart_led_command();
    // }

    while (true);
    
    return 0;
}
