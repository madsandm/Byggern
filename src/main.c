#include <stdio.h>
#include "utilities.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/gpio.h"
#include "drivers/adc.h"

int main() {
    uart.init(MY_UBRR, true);
    sram.init();
    adc.init();

    // Enable interrupts
    sei();

    printf("System initialized.\n");

    //sram.test();

    while (true) {
        adc.read();

        printf("ADC Channels: ");
        for (uint8_t i = 0; i < NUMBER_OF_ADC_CHANNELS; i++) {
            uint8_t value = adc.getChannel(i);
            printf("%u ", value);
        }
        printf("\n");

        _delay_ms(50);
    }
    // adc_init();

    // test_latch();

    // GPIO.initPin(&DDRB, PB0, OUTPUT);
    // while (true) {
    //     GPIO.togglePin(&PORTB, PB0); // Toggle LED connected to PB0
    //     delay(500);
    // }


    // while (1){
    //     uart_led_command();
    // }

    while (true);
    
    return 0;
}
