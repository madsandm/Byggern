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

void blinky(){
    // Implement LED blinking functionality here
    initialize_pin(&DDRB, PB0, OUTPUT);
    while(1){
        toggle_pin(&PORTB, PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 100000; i++); // Simple delay
        
    }
}

void uart_led(){
    volatile uint8_t data = uart_receive();
    uart_transmit(data);
    if (data == '1') {
        set_pin(&PORTB, PB0);
    } else {
        clear_pin(&PORTB, PB0);
    }
}
