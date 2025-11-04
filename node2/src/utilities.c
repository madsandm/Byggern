#include "sam.h"
#include "can_controller.h"
#include "pwm.h"
#include "controller.h"
#include "motor_driver.h"
#include "solenoid.h"
#include "can_interrupt.h"

uint32_t can_joystick_to_us(){
    volatile int x;
    volatile uint32_t y;
    volatile uint32_t b;
    CAN_MESSAGE msg_tx;
    msg_tx.id = 1;
    msg_tx.data_length = 3;
    msg_tx.data[0] = 1;
    msg_tx.data[1] = 2;
    msg_tx.data[2] = 3;
    while (1){
        CAN_MESSAGE msg_rx;
        if (RX_MB1_FLAG){
            can_receive(&msg_rx, 1);
            RX_MB1_FLAG = 0;
            CAN0->CAN_IER = CAN_IDR_MB1;
            //printf("Received message in MB1\n\r");
            can_send(&msg_tx, 0);
        } 
        if (RX_MB2_FLAG){
            can_receive(&msg_rx, 2);
            RX_MB2_FLAG = 0;
            CAN0->CAN_IER = CAN_IDR_MB2;
            //printf("Received message in MB2\n\r");
            can_send(&msg_tx, 0);
        }
        x = -(msg_rx.data[0]-130);
        y = ((msg_rx.data[1]) * 47 + 9000)/10;
        b = msg_rx.data[2];
        motorController_setTarget(x * 4 + 450);
        pwm_set_duty_us(1,y);
        set_solenoid(b);
        // printf("%d %d %d ", x,y,b);
    }

}