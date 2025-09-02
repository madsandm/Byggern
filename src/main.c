#include <stdio.h>
#include <avr/io.h>
#include "utilities.h"
#include "uart.h"

int main() {
    uart_init(9600);
    initialize_pin(&DDRB, PB0, OUTPUT);
    blinky(3);
    uart_transmit('7');

    while (1){
        uart_led();
    }
    
    return 0;
}