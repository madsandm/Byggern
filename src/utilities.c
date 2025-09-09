#include "utilities.h"
#include "uart.h"
#include <avr/io.h>
#include <stdint.h>
#include "sram.h"


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

void uart_led_command(){
    while (uart.available()) {
        char command = uart.read();
        uart.write(command);
        if (command == '1') {
            clear_pin(&PORTB, PB0); // Turn on LED
            uart.println("\nLED ON");
        } else if (command == '0') {
            set_pin(&PORTB, PB0); // Turn off LED
            uart.println("\nLED OFF");
        }
    }
}

void test_latch(){
    uint32_t iterations = 30000;
    sram.data[1] = 0xFF;
    
    for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
    sram.data[2] = 0xFF;

    for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
    sram.data[1] = 0xF0;

    for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
    sram.data[2] = 0xF0;
}