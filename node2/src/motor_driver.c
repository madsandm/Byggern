#include "sam.h"
#include "pwm.h"

#define MOTOR_LIMIT 999

void motor_driver_init(){
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    PIOC->PIO_SODR = PIO_PC23; // Set direction pin low (forward)
    pwm_channel_init(0);
}

void motor_driver_set_vel(int x){
    if (x > MOTOR_LIMIT) {
        x = MOTOR_LIMIT;
    }

    if (x < -MOTOR_LIMIT) {
        x = -MOTOR_LIMIT;
    }

    if (x < 0){
        PIOC->PIO_SODR |= PIO_PC23;
    } else {
        PIOC->PIO_CODR |= PIO_PC23;
    }

    int vel = abs(x) * 20;
    //printf("%d   %d\n", pos, (PIOB->PIO_ODSR & PIO_PC24) > 0);
    pwm_set_duty_us_unrestricted(0, vel);
}