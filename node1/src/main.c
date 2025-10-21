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
#include "menu.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();
    spi.init();
    //oled.init();
    canbus.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    char data[] = "Hello!";

    while (true) {
        canbus.transmit((CanbusPacket){
            .id = 13,
            .data = data,
            .size = sizeof(data)
        });

        // CanbusPacket response = canbus.receive();
        // printf("ID: %d, Size: %d, data: ", response.id, response.size);
        // for (char i = 0; i < response.size; i++) {
        //     printf("%c", response.data[i]);
        // }
        // printf("\n");
        // free(response.data);
        printf("Transmit\n");

        _delay_ms(100);
    }
    while(1);

    //adc.printChannels();

    //ioGrid.calibrate(touchpad);

    //_delay_ms(1000);

    //menu.init();
    //menu.show(&mainMenu);
    
    //oled.circle(6, 30, 10);
    oled.sram_init();
    oled.sram_flush();
    
    menu.init();
    menu.show(&mainMenu);
    //etch_a_sketch();
    
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
