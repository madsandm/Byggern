#include "sam.h"
#include "pwm.h"


void motor_driver_init(){
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    pwm_channel_init(0);
}

void motor_driver_set_vel(int x){
    if (x > 1000) {
        x = 1000;
    }

    if (x < -1000) {
        x = -1000;
    }

    if (x < 0){
        PIOC->PIO_SODR = PIO_PC23;
    } else {
        PIOC->PIO_CODR = PIO_PC23;
    }

    int vel = abs(x) * 20;
    //printf("%d   %d\n", pos, (PIOB->PIO_ODSR & PIO_PC24) > 0);
    pwm_set_duty_us_unrestricted(0, vel);
}