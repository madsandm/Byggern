#ifndef PWM_H
#define PWM_H

#include "sam.h"


#define SERVO_PIN PIO_PB13
#define SERVO_PORT PORTB
#define FREQ 50

void pwm_pin_setup();
void pwm_init(uint8_t channel);
void pwm_enable(uint8_t channel);
void pwm_disable(uint8_t channel);
void pwm_set_duty_us(uint8_t channel, uint32_t us);
void servo_init();

//uint32_t can_to_pwm(int joystick_input);


#endif
