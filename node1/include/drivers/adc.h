#ifndef ADC_H
#define ADC_H

#define ADC_BASE_ADDRESS 0x1000
#define NUMBER_OF_ADC_CHANNELS 4

#include <stdint.h>

typedef struct {
    void (*init)();
    void (*read)();
    uint8_t (*getChannel)(uint8_t channel);
    uint8_t (*readChannel)(uint8_t channel);
    void (*printChannels)();
} IADC;

IADC adc;

#endif // ADC_H