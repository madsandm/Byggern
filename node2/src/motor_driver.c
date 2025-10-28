#include "sam.h"
#include "pwm.h"


void motor_driver_init(){
    PMC->PMC_PCER0 |= (1 << ID_PIOB);       // Enable clock for PIOB
    PIOB->PIO_PDR = PIO_PDR_P12;            // Disable PIO
    PIOB->PIO_ABSR = (1 << 12);             // SET PWM to PB13
    PIOB->PIO_PUDR = PIO_PUDR_P12;          // Disable pull-up resistor
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    pwm_channel_init(0);
}

void motor_driver_set_pos(uint32_t x){
    volatile uint32_t pos;
    x = x-2;
    if (x > 127){
        pos = (x - 128) * 156;
        PIOC->PIO_SODR = PIO_PC23;
    } else {
        pos = (128 - x) * 156;
        PIOC->PIO_CODR = PIO_PC23;
    }
    //printf("%d   %d\n", pos, (PIOB->PIO_ODSR & PIO_PC24) > 0);
    pwm_set_duty_us_unrestricted(0,pos);
}