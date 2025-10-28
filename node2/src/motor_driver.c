#include "sam.h"
#include "pwm.h"


void motor_driver_init(){
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    pwm_channel_init(0);
}

void motor_driver_set_pos(uint32_t x){
    volatile uint32_t vel;
    x = x-2;
    if (x > 127){
        vel = (x - 128) * 156;
        PIOC->PIO_SODR = PIO_PC23;
    } else {
        vel = (128 - x) * 156;
        PIOC->PIO_CODR = PIO_PC23;
    }
    //printf("%d   %d\n", pos, (PIOB->PIO_ODSR & PIO_PC24) > 0);
    pwm_set_duty_us_unrestricted(0, vel);
}