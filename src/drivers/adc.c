#include "drivers/adc.h"
#include "drivers/timer.h"
#include <stdbool.h>

void adc_init() {
    volatile char* adc = (char*)ADC_BASE_ADDRESS;

}