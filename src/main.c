#include <stdio.h>
#include <avr/io.h>
#include "utilities.h"
#include "uart.h"

int main() {
    uart_init(MY_UBRR);
    uart_printf_enable();
    
    initialize_pin(&DDRB, PB0, OUTPUT);
    blinky(3);

    while (1){
        uart_led();
    }
    
    return 0;
}