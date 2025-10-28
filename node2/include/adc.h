#ifndef _ADC_H_
#define _ADC_H_
#include "sam.h"

void adc_init(void);
void ADC_Handler(void);

extern uint8_t IR_flag;

#endif // _ADC_H_