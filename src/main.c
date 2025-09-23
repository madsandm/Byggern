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

    char spi_setup_commands[] = {
        0xae,
        0xa1,
        0xda,
        0x12,
        0xc8,
        0xa8,
        0x3f,
        0xd5,
        0x80,
        0x81,
        0x50,
        0xd9,
        0x21,
        0x20,
        0x02,
        0xdb,
        0x30,
        0xad,
        0x00,
        0xa4,
        0xa6,
        0xaf
    };

    send_data(spi.stream, spi_setup_commands, sizeof(spi_setup_commands));

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
