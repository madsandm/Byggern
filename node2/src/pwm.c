
#include "sam.h"

#define PWM_MAX_DUTY_CYCLE 21
#define PWM_MIN_DUTY_CYCLE 9


void pwm_pin_setup(){
    PMC->PMC_PCER0 |= (1 << ID_PIOB);       // Enable clock for PIOB
    PIOB->PIO_PDR |= PIO_PDR_P13;            // Disable PIO
    PIOB->PIO_ABSR |= (1 << 13);             // SET PWM to PB13
    PIOB->PIO_PUDR |= PIO_PUDR_P13;          // Disable pull-up resistor

    PIOB->PIO_PDR |= PIO_PDR_P12;            // Disable PIO
    PIOB->PIO_ABSR |= (1 << 12);             // SET PWM to PB13
    PIOB->PIO_PUDR |= PIO_PUDR_P12;          // Disable pull-up resistor
}


void pwm_init(){
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));                 // Attach clock to pwm
    PWM->PWM_CLK = (PWM_CLK_PREA(0)) | PWM_CLK_DIVA(84);    // Sets pwm clock
}

void pwm_channel_init(uint8_t channel){
    PWM->PWM_DIS = (1 << channel);
    PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[channel].PWM_CPRD = 20000;
    PWM->PWM_CH_NUM[channel].PWM_CDTY = 1500;
    PWM->PWM_ENA = (1 << channel);
}


void pwm_enable(uint8_t channel){
    PWM->PWM_ENA = (1 << channel);
}

void pwm_disable(uint8_t channel){
    PWM->PWM_DIS = (1 << channel);
}

void pwm_set_duty_us(uint8_t channel, uint32_t us){
    if (us > 2100 || us < 900){
        PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = 1500;
    }else{
        PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = us;
    }
}

void pwm_set_duty_us_unrestricted(uint8_t channel, uint32_t us){
    PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = us;
}

void servo_init(){
    pwm_channel_init(1);
    pwm_set_duty_us(1, 1500);
}
