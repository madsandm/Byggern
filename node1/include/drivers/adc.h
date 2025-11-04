#ifndef ADC_H
#define ADC_H

#define ADC_BASE_ADDRESS 0x1000
#define NUMBER_OF_ADC_CHANNELS 4

#include <stdint.h>

void adc_init();
void adc_read();
uint8_t adc_getChannel(uint8_t channel);
uint8_t adc_readChannel(uint8_t channel);
void adc_printChannels();

#endif // ADC_H