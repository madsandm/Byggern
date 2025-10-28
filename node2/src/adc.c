#include "adc.h"
#include "sam.h"

uint8_t IR_flag = 0;

void adc_init() {
    // Initialize the ADC peripheral
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Enable ADC clock
    PMC->PMC_PCER0 |= (1u << ID_PIOA);

    PIOA->PIO_PDR = PIO_PDR_P16;   // disable PIO, hand pin to ADC
    PIOA->PIO_ODR = PIO_ODR_P16;   // input

    ADC->ADC_CHDR = 0xFFFF; // Disable all channels
    ADC->ADC_CHER = ADC_CHER_CH0; // Enable channel 0

    ADC->ADC_MR = ADC_MR_FREERUN_ON 
                | ADC_MR_PRESCAL(10) 
                | ADC_MR_STARTUP_SUT64 
                | ADC_MR_TRACKTIM(15);

    ADC->ADC_CWR = ADC_CWR_LOWTHRES(0x9B2); // Set low threshold (2^12 / 3.3 * 2 = 0x9B2)
    ADC->ADC_EMR = ADC_EMR_CMPMODE_LOW
                 | ADC_EMR_CMPSEL(0)
                 | ADC_EMR_CMPFILTER(0);

    ADC->ADC_IER = ADC_IER_COMPE; // Enable comparison event interrupt
    NVIC_EnableIRQ(ADC_IRQn); // Enable ADC interrupt in NVIC

    ADC->ADC_CR = ADC_CR_START; // Start ADC
}

void ADC_Handler(void) {
    uint32_t isr = ADC->ADC_ISR; // Read and clear interrupt status

    if (isr & ADC_ISR_COMPE) {
        // Comparison event occurred (IR sensor blocked)
        // Set flag here
        IR_flag = 1;
    }
}