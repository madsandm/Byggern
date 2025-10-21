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
    int counter = 0;
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
        oled.draw_square(col, row, 3);
        //_delay_us(10);
        if (io.read_buttons(0) & (1 << 5)) {
            break; // Exit the loop if button 0 is pressed
        }
        if (counter++ > 20) {
            counter = 0;
            oled.present();
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
    oled.clear();
    oled.present();

    while (true) {
        // Display scores
        oled.erase_area(8, 0, 120, 63); // Clear ball area
        oled.pos(0, 50);

        oled.print(itoa(score1, NULL, 10));
        oled.print(" - ");
        oled.print(itoa(score2, NULL, 10));

        // Read joystick positions to move paddles
        paddle1_y = (256 - io.read_joystick(1)*12/10) * 8 / 32;
        paddle2_y = (256 - io.read_touchpad(1)) * 8 / 32;

        //draw paddles and ball
        oled.erase_area(0, 0, 7, 63); // Clear left paddle area
        oled.draw_square(0, paddle1_y, 8);
        oled.draw_square(0, paddle1_y + 8, 8);

        oled.erase_area(120, 0, 127, 63); // Clear right paddle area
        oled.draw_square(120, paddle2_y, 8);
        oled.draw_square(120, paddle2_y + 8, 8);

        oled.circle(ball_x, ball_y, 3);

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
            oled.clear();
            oled.pos(24, 32);
            if (score1 >= 5) {
                oled.print("Player 1 Wins!");
            } else {
                oled.print("Player 2 Wins!");
            }
            oled.present();
            _delay_ms(4000);
            break;
        }

        _delay_ms(10);
        oled.present();
        if (io.read_buttons(0) & (1 << 5)) {
            break; // Exit the loop if button 0 is pressed
        }
    }
}


void can_joystick(){
    while (true){
        IPosition position = ioGrid.getPosition(joystick);
        uint8_t j_h = io.read_joystick(0);
        uint8_t j_v = io.read_joystick(1);
        uint8_t j_b = io.read_joystick(2);
        uint8_t data[3] = {
            position.x + 128,
            position.y + 128,
            j_b
        };
        canbus.transmit(canbus.create_packet(24, data, sizeof(data)));

        if (io.read_buttons(0) & (1 << 5)) {
            break; // Exit the loop if button 0 is pressed
        }
        _delay_ms(60);
        oled.clear();
        oled.print(itoa(j_h,NULL, 10));
        oled.present();


        printf("sent joystick\n");
    }
}

void send_data(FILE* stream, const char* data, size_t size) {
    fwrite(data, 1, size, stream);
}
