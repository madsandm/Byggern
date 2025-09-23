#include <stdio.h>
#include "utilities.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
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

    
    spi.slave_select(&PORTB, DISPLAY_CS);
    GPIO.setPin(&PORTB, DISPLAY_DC); // Command mode
    spi.transmit(0b00011000);
    spi.transmit(0b00011000);
    spi.slave_deselect(&PORTB, DISPLAY_CS);
    
    
    //oled.clear();
    //oled.draw_pixel(30, 30, 1);

    int i = 0;
    bool toggle = true;
    while (true) {

        spi.slave_select(&PORTB, PB4);
        spi.transmit(0x05);
        spi.transmit(i);
        spi.transmit(toggle);
        spi.slave_deselect(&PORTB, PB4);

        i++;
        if(i >= 6){
            i = 0;
            toggle = !toggle;
        }

        _delay_ms(100);
    }

    //adc.printChannels();

    while (true);
    
    return 0;
}
