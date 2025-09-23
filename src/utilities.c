#include "utilities.h"
#include "drivers/uart.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "drivers/sram.h"
#include "drivers/gpio.h"

void blinky(uint8_t times){
    // Implement LED blinking functionality here
    for(uint8_t i = 0; i < times*2; i++){
        GPIO.togglePin(&PORTB, PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 30000; i++); // Simple delay
    }
}

void uart_led_command(){
    while (uart.available()) {
        char command = uart.read();
        uart.write(command);
        if (command == '1') {
            GPIO.clearPin(&PORTB, PB0); // Turn on LED
            uart.println("\nLED ON");
        } else if (command == '0') {
            GPIO.setPin(&PORTB, PB0); // Turn off LED
            uart.println("\nLED OFF");
        }
    }
}

void test_latch(){
    uint32_t iterations = 300000;
    printf("Start latch test\n");

    while (true) {
        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram.data[1] = 0xFF;
        
        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram.data[2] = 0xFF;

        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram.data[1] = 0;

        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram.data[2] = 0;
    }

    printf("\nLatch test completed.\n");
}

void send_data(FILE* stream, const char* data, size_t size) {
    fwrite(data, 1, size, stream);
}
