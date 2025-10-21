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
#include "drivers/canbus.h"
#include "drivers/mcp2515.h"
#include "drivers/io.h"
#include "menu.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();
    spi.init();
    oled.init();
    canbus.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    /* CanbusPacket response = canbus.receive();
    printf("ID: %d, Size: %d, data: ", response.id, response.size);
    for (char i = 0; i < response.size; i++) {
        printf("%c", response.data[i]);
    }
    printf("\n");
    free(response.data); */

    _delay_ms(10);
    
    oled.sram_init();
    oled.sram_flush();
    
    
    menu.init();
    menu.show(&mainMenu);
    
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


    while (true);
    
    return 0;
}
