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

    while (true) {
        spi.slave_select(&PORTB, PB4);
        spi.transmit(0xAA);
        spi.slave_deselect(&PORTB, PB4);

        printf("Sent: 0xAA\n");
        _delay_ms(10);
    }

    //adc.printChannels();

    while (true);
    
    return 0;
}
