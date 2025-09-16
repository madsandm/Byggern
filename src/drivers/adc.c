#include "drivers/adc.h"
#include "utilities.h"
#include <stdbool.h>
#include <util/delay.h>
#include "drivers/gpio.h"
#include "drivers/sram.h"

volatile uint8_t* adcMemory = (uint8_t*)ADC_BASE_ADDRESS;
uint8_t lastResult[NUMBER_OF_ADC_CHANNELS];

void adc_init() {

    GPIO.initPin(&DDRD, PD5, OUTPUT); // OC0 pin as output

    OCR1A = 1;
    TCCR1A = 
        (1 << COM1A0) | // Toggle OC1A on compare match
        (1 << WGM10) | (1 << WGM11); // PWM phase correct 10-bit
    TCCR1B =
        (1 << WGM12) | (1 << WGM13) | // CTC
        (1 << CS11); // No prescaling
}

void adc_read() {

    adcMemory[0] = 0xFF; // start the reading

    _delay_us(4);

    for (int i = 0; i < NUMBER_OF_ADC_CHANNELS; i++) {
        lastResult[i] = adcMemory[0];
    }
}

uint8_t adc_getChannel(uint8_t channel) {
    return lastResult[channel];
}

uint8_t adc_readChannel(uint8_t channel) {
    adc_read();
    return adc_getChannel(channel);
}

IADC adc = {
    .init = adc_init,
    .read = adc_read,
    .getChannel = adc_getChannel,
    .readChannel = adc_readChannel
};