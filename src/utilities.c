#include "utilities.h"
#include "uart.h"
#include <avr/io.h>
#include <stdint.h>


void initialize_pin(volatile uint8_t* DD, uint8_t pin, uint8_t mode) {
    *DD |= (mode << pin);
}

void toggle_pin(volatile uint8_t* port, uint8_t pin) {
    // Toggle the specified pin on the specified port
    *port ^= (1 << pin);
}

void set_pin(volatile uint8_t* port, uint8_t pin) {
    *port |= (1 << pin);
}

void clear_pin(volatile uint8_t* port, uint8_t pin) {
    *port &= ~(1 << pin);
}

void blinky(uint8_t times){
    // Implement LED blinking functionality here
    for(uint8_t i = 0; i < times*2; i++){
        toggle_pin(&PORTB, PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 30000; i++); // Simple delay
    }
}

void uart_led(){
    char data = uart.read();
    uart.write(data);

    if (data) {
        toggle_pin(&PORTB, PB0);
    }
}