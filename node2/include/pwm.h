#ifndef PWM_H
#define PWM_H

#include "sam.h"

void pwm_pin_setup();
void pwm_init();
void pwm_channel_init(uint8_t channel);
void pwm_enable(uint8_t channel);
void pwm_disable(uint8_t channel);
void pwm_set_duty_us(uint8_t channel, uint32_t us);
void pwm_set_duty_us_unrestricted(uint8_t channel, uint32_t us);
void servo_init();

#endif
