#include "utilities.h"
#include "drivers/uart.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "drivers/sram.h"
#include "drivers/gpio.h"
#include "drivers/io.h"
#include "drivers/oled.h"

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

void etch_a_sketch() {
    oled.clear();
    while (true) {
        if (io.read_buttons(2)) {
            oled.clear();
        }
        uint8_t x = io.read_touchpad(0);
        uint8_t y = io.read_touchpad(1);
        uint8_t row = ((256 - y) * 8) / 32;
        uint8_t col = (x * 128) / 256;
        if (col > 127) col = 127;
        oled.pos(row, col);
        oled.draw_square(col, row, 8);
        _delay_us(100);
        if (io.read_buttons(0)) {
            break; // Exit the loop if button 0 is pressed
        }
    }
}

void pong() {
    // Implement a simple Pong game here
    uint8_t ball_x = 64;
    uint8_t ball_y = 32;
    int8_t ball_dx = 4;
    int8_t ball_dy = 1;
    uint8_t paddle1_y = 28;
    uint8_t paddle2_y = 28;
    uint8_t score1 = 0;
    uint8_t score2 = 0;
    oled.clear();

    while (true) {

        // Read joystick positions to move paddles
        paddle1_y = (256 - io.read_joystick(1)) * 8 / 32;
        paddle2_y = (256 - io.read_touchpad(1)) * 8 / 32;

        //draw paddles and ball
        oled.line(0, paddle1_y-4, 0, paddle1_y + 3);
        oled.line(1, paddle1_y-4, 1, paddle1_y + 3);
        oled.line(126, paddle2_y-4, 126, paddle2_y + 3);
        oled.line(127, paddle2_y-4, 127, paddle2_y + 3);
        oled.draw_square(ball_x, ball_y, 2);

        // Update ball position
        ball_x += ball_dx;
        ball_y += ball_dy;

        // Check for collisions with walls
        if (ball_x == 0 || ball_x >= 127) ball_dx = -ball_dx;
        if (ball_y == 0 || ball_y >= 63) ball_dy = -ball_dy;

        _delay_ms(100);
        oled.clear();
    }
}


void send_data(FILE* stream, const char* data, size_t size) {
    fwrite(data, 1, size, stream);
}
