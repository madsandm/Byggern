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
    uart_init(MY_UBRR, true);
    sram_init();
    adc_init();
    spi_init();
    oled_init();
    canbus_init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    //squash_game();

    _delay_ms(10);

    oled_sram_init();
    oled_sram_flush();

    menu_init();
    menu_show(&mainMenu);

    while (true);
    
    return 0;
}
