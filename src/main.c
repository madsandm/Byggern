#include <stdio.h>
#include "utilities.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/gpio.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/spi.h"
#include "drivers/oled.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();
    spi.init();
    oled.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    //adc.printChannels();

    //ioGrid.calibrate(touchpad);

    _delay_ms(1000);

    /* 
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0xa6);
    _delay_ms(1000);
    spi.transmit(0xa7);
    spi.slave_deselect(&PORTB, DISPLAY_CS);
     */
    
    oled.clear();
    //oled.draw_pixel(30, 30, 1);

    oled.pos(4,60);
    oled.print("Hello, World!");

    uint8_t row = 0;
    uint8_t col = 0;
    uint8_t x, y, button;
    while (true) {
        spi.slave_select(&PORTB, IO_BOARD_CS);
        spi.transmit(0x03);
        _delay_us(40);
        spi.transmit_receive(&x);
        _delay_us(2);
        spi.transmit_receive(&y);
        _delay_us(2);
        spi.transmit_receive(&button);
        _delay_us(2);
        printf("Received from IO Board: %d, %d, %d\n", x, y, button);
        _delay_ms(10);
        spi.slave_deselect(&PORTB, IO_BOARD_CS);

        row = ((256 - y)*8)/256;
        col = ((x) * 128)/256 - sizeof("Hello, World!") * 2;
        if (row > 8) row = 7;
        if (col > 127) col = 127;
        oled.clear();
        oled.pos(row, col);
        oled.print("Hello, World!");
        _delay_ms(1);
    }


    int i = 0;
    bool toggle = true;
    while (true) {

        spi.slave_select(&PORTB, IO_BOARD_CS);
        spi.transmit(0x05);
        spi.transmit(i);
        spi.transmit(toggle);
        spi.slave_deselect(&PORTB, IO_BOARD_CS);

        i++;
        if(i >= 6){
            i = 0;
            toggle = !toggle;
        }

        _delay_ms(50);
    }

    //adc.printChannels();

    while (true);
    
    return 0;
}
