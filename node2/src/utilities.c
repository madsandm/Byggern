#include "sam.h"
#include "can_controller.h"
#include "pwm.h"
#include "controller.h"
#include "motor_driver.h"

uint32_t can_joystick_to_us(){
    volatile int x;
    volatile uint32_t y;
    volatile uint32_t b;
    while (1){
        CAN_MESSAGE msg_rx;
        if (can_receive(&msg_rx, 0) == 0){
            x = -(msg_rx.data[0]-130);
            y = ((msg_rx.data[1]) * 47 + 9000)/10;
            b = msg_rx.data[2];
            //motorController_setTarget(x);
            motor_driver_set_vel(x);
            pwm_set_duty_us(1,y);
            printf("%d %d %d\n", x,y,b);
        }
    }
    
}