#include "utilities.h"
#include "drivers/uart.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "drivers/sram.h"
#include "drivers/gpio.h"
#include "drivers/io.h"
#include "drivers/oled.h"
#include "drivers/canbus.h"
#include "drivers/joystick.h"
#include <stdlib.h>

void blinky(uint8_t times){
    // Implement LED blinking functionality here
    for(uint8_t i = 0; i < times*2; i++){
        gpio_togglePin(&PORTB, PB0); // Toggle LED connected to PB0
        for(volatile uint32_t i = 0; i < 30000; i++); // Simple delay
    }
}

void uart_led_command(){
    while (uart_available()) {
        char command = uart_read();
        uart_write(command);
        if (command == '1') {
            gpio_clearPin(&PORTB, PB0); // Turn on LED
            uart_println("\nLED ON");
        } else if (command == '0') {
            gpio_setPin(&PORTB, PB0); // Turn off LED
            uart_println("\nLED OFF");
        }
    }
}

void test_latch(){
    uint32_t iterations = 300000;
    printf("Start latch test\n");

    while (true) {
        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram_data[1] = 0xFF;
        
        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram_data[2] = 0xFF;

        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram_data[1] = 0;

        for (volatile uint32_t i = 0; i < iterations; i++); // Simple delay
        printf(".");
        sram_data[2] = 0;
    }

    printf("\nLatch test completed.\n");
}

void etch_a_sketch() {
    oled_clear();
    int counter = 0;
    while (true) {
        if (io_readButtons(2)) {
            oled_clear();
        }
        uint8_t x = io_readTouchpad(0);
        uint8_t y = io_readTouchpad(1);
        uint8_t row = ((256 - y) * 8) / 32;
        uint8_t col = (x * 128) / 256;
        if (col > 127) col = 127;
        oled_pos(row, col);
        oled_draw_square(col, row, 3);
        //_delay_us(10);
        if (io_readButtons(0) & (1 << 5)) {
            break; // Exit the loop if button 0 is pressed
        }
        if (counter++ > 20) {
            counter = 0;
            oled_present();
        }
    }
}

void pong() {
    // Implement a simple Pong game here
    uint8_t ball_x = 64;
    uint8_t ball_y = 32;
    int8_t ball_dx = 4;
    int8_t ball_dy = 2;
    uint8_t paddle1_y = 28;
    uint8_t paddle2_y = 28;
    uint8_t score1 = 0;
    uint8_t score2 = 0;
    oled_clear();
    oled_present();

    while (true) {
        // Display scores
        oled_erase_area(8, 0, 120, 63); // Clear ball area
        oled_pos(0, 50);

        oled_print(itoa(score1, NULL, 10));
        oled_print(" - ");
        oled_print(itoa(score2, NULL, 10));

        // Read joystick positions to move paddles
        paddle1_y = (256 - io_readJoystick(1)*12/10) * 8 / 32;
        paddle2_y = (256 - io_readTouchpad(1)) * 8 / 32;

        //draw paddles and ball
        oled_erase_area(0, 0, 7, 63); // Clear left paddle area
        oled_draw_square(0, paddle1_y, 8);
        oled_draw_square(0, paddle1_y + 8, 8);

        oled_erase_area(120, 0, 127, 63); // Clear right paddle area
        oled_draw_square(120, paddle2_y, 8);
        oled_draw_square(120, paddle2_y + 8, 8);

        oled_circle(ball_x, ball_y, 3);

        // Update ball position
        ball_x += ball_dx;
        ball_y += ball_dy;

        // Check for collisions with paddles
        if (ball_x <= (8 + 3) && ball_y >= paddle1_y && ball_y <= paddle1_y + 16) {
            ball_dx = -ball_dx;
            ball_dy = TCNT1 % 5 - 5; // Randomize vertical direction
            ball_x = (8 + 3); // Prevent sticking to paddle
        }
        if (ball_x >= (120 - 3) && ball_y >= paddle2_y && ball_y <= paddle2_y + 16) {
            ball_dx = -ball_dx;
            ball_x = (119 - 3); // Prevent sticking to paddle
        }

        // Check for scoring
        if (ball_x <= 1) {
            score2++;
            ball_x = 64;
            ball_y = 32;
            ball_dx = -ball_dx; // Change direction
            ball_dy = TCNT1 % 5 - 5; // Randomize vertical direction
        }
        if (ball_x >= 123) {
            score1++;
            ball_x = 64;
            ball_y = 32;
            ball_dx = -ball_dx; // Change direction
            ball_dy = TCNT1 % 5 - 5; // Randomize vertical direction
        }

        // Check for collisions with top and bottom walls
        if (ball_y <= 0 || ball_y >= 60) {
            ball_dy = -ball_dy;
        }

        // Game over
        if (score1 >= 5 || score2 >= 5) {
            oled_clear();
            oled_pos(24, 32);
            if (score1 >= 5) {
                oled_print("Player 1 Wins!");
            } else {
                oled_print("Player 2 Wins!");
            }
            oled_present();
            _delay_ms(4000);
            break;
        }

        _delay_ms(10);
        oled_present();
        if (io_readButtons(0) & (1 << 5)) {
            break; // Exit the loop if button 0 is pressed
        }
    }
}


void can_joystick(){
    while (true){
        IPosition position = IODevice_getPosition(joystick);
        uint8_t j_h = io_readJoystick(0);
        uint8_t j_v = io_readJoystick(1);
        uint8_t j_b = io_readJoystick(2);
        uint8_t data[3] = {
            position.x + 128,
            position.y + 128,
            j_b
        };
        canbus_transmit(canbus_createPacket(24, data, sizeof(data)));

        if (io_readButtons(0) & (1 << 5)) {
            printf("breaking loop");
            break; // Exit the loop if button 0 is pressed
        }
        _delay_ms(100);
        printf("sent joystick\n");
    }
}

void send_data(FILE* stream, const char* data, size_t size) {
    fwrite(data, 1, size, stream);
}
