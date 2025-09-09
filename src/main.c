#include <stdio.h>
#include <avr/io.h>
#include "utilities.h"
#include "uart.h"

int main() {
    uart.init(MY_UBRR, true);

    initialize_pin(&DDRB, PB0, OUTPUT);
    blinky(3);

    while (1){
        uart_led();
    }
    
    return 0;
}