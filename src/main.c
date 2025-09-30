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
    // oled.init();
    canbus.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    mcp2515.reset();
    _delay_ms(10);
    mcp2515.dump_memory();

    uint8_t data[] = {
        'A',
        'B'
    };

    while (true) {
        canbus.transmit((CanbusPacket){
            .id = 13,
            .data = data,
            .size = sizeof(data)
        });

        mcp2515.dump_memory();
        printf("Packet sent\n");

        _delay_ms(100);
    }

    while(true);

    _delay_ms(1000);

    CanbusPacket response = canbus.receive();
    printf("ID: %d, Size: %d, data: ", response.id, response.size);
    for (char i = 0; i < response.size; i++) {
        printf("%c", response.data[i]);
    }
    printf("\n");
    free(response.data);
    while (true);

    //adc.printChannels();

    //ioGrid.calibrate(touchpad);

    _delay_ms(1000);

    //menu.init();
    //menu.show(&mainMenu);
    

    //oled.clear();
    //oled.circle(6, 30, 10);
    oled.sram_init();
    oled.sram_flush();

    menu.init();
    menu.show(&mainMenu);
    //etch_a_sketch();

/*     int j = 0;
    while(true){
        oled.draw_pixel(10, 10);
        oled.draw_square(20, 20, 5);
        oled.line(30, 30, 100, 30);
        oled.circle(50 + j, 50, 10);

        oled.sram_swap();
        oled.sram_flush();
        _delay_ms(1000);

        j++;
    } */
    

    
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
