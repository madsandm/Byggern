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

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();
    spi.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    //adc.printChannels();

    //ioGrid.calibrate(touchpad);

    _delay_ms(1000);

    spi.slave_select(&PORTB, PB1);
    spi.transmit(0xae);
    spi.transmit(0xa1);
    spi.transmit(0xda);
    spi.transmit(0x12);
    spi.transmit(0xc8);
    spi.transmit(0xa8);
    spi.transmit(0x3f);
    spi.transmit(0xd5);
    spi.transmit(0x80);
    spi.transmit(0x81);
    spi.transmit(0x50);
    spi.transmit(0xd9);
    spi.transmit(0x21);
    spi.transmit(0x20);
    spi.transmit(0x02);
    spi.transmit(0xdb);
    spi.transmit(0x30);
    spi.transmit(0xad);
    spi.transmit(0x00);
    spi.transmit(0xa4);
    spi.transmit(0xa6);
    spi.transmit(0xaf);
    spi.slave_deselect(&PORTB, PB1);


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
