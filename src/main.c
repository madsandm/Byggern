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

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    //adc.printChannels();

    // ioGrid.calibrate(touchpad);

    while (true) {
        IPosition pos = ioGrid.getPosition(joystick);
        printf("Joystick Position: X=%d, Y=%d\n", pos.x, pos.y);
        DeviceDirection direction = ioGrid.getDirection(joystick);
        switch (direction) {
            case LEFT:
                printf("Direction: LEFT\n");
                break;
            case RIGHT:
                printf("Direction: RIGHT\n");
                break;
            case UP:
                printf("Direction: UP\n");
                break;
            case DOWN:
                printf("Direction: DOWN\n");
                break;
            case NEUTRAL:
                printf("Direction: NEUTRAL\n");
                break;
        }
        _delay_ms(200);
    }

    //adc.printChannels();

    while (true);
    
    return 0;
}
