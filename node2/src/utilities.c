#include "utilities.h"
#include "sam.h"
#include "can_controller.h"
#include "pwm.h"
#include "controller.h"
#include "motor_driver.h"
#include "solenoid.h"
#include "can_interrupt.h"
#include "time.h"
#include "adc.h"
#include "game_state.h"

uint32_t game_start_time;
uint32_t game_time;
uint32_t lives;
uint32_t game_freeze_time;
CAN_MESSAGE last_msg_tx;
GAME_STATE game_state;

void transition_to_state(GAME_STATE state) {
    if (game_state == state && state != IDLE) {
        return;
    }
    printf("Switching state %d -> %d\n", game_state, state);

    // CLEAN UP LAST STATE
    switch(game_state) {
        case IDLE:
            break;
        case PLAYING:
            IR_flag = 0;
            break;
        case LIFE_LOST:
            break;
        case GAME_OVER:
            break;
    }

    // INIT NEW STATE
    switch(state) {
        case IDLE:
            score_init();
            motorController_setTarget(500);
            set_solenoid(0);
            break;
        case PLAYING:
            NVIC_EnableIRQ(ADC_IRQn);
            IR_flag = 0;
            break;
        case LIFE_LOST:
            lives--;
            set_solenoid(0);
            break;
        case GAME_OVER:
            break;
    }
    game_state = state;
}

uint32_t can_joystick_to_us(){
    transition_to_state(IDLE);

    int x;
    uint32_t y;
    uint32_t button;

    CAN_MESSAGE msg_tx;
    bool new_message = false;

    while (1) {
        CAN_MESSAGE msg_rx;
        if (RX_MB1_FLAG){
            can_receive(&msg_rx, 1);
            RX_MB1_FLAG = 0;
            CAN0->CAN_IER = CAN_IDR_MB1;
            new_message = true;
        } 
        if (RX_MB2_FLAG){
            can_receive(&msg_rx, 2);
            RX_MB2_FLAG = 0;
            CAN0->CAN_IER = CAN_IDR_MB2;
            new_message = true;
        }

        if (new_message) {
            printf("New message received, id: %d ", msg_rx.id);

            if (msg_rx.id == CANMSG_SCOREBOARD_RESET) {
                transition_to_state(IDLE);
                printf("RESETTING...");
            }

            if (msg_rx.id == CANMSG_JOYSTICK) {
                x = -(msg_rx.data[0]-130);
                y = ((msg_rx.data[1]) * 47 + 9000)/10;
                button = msg_rx.data[2];

                pritnf("JOYSTICK: %d %d %d", x, y, button);
            }

            printf("\n");
        }

        switch(game_state) {
            case IDLE:
                break;
                if (button) {
                    set_solenoid(1);
                    transition_to_state(PLAYING);
                }
            case PLAYING: {

                motorController_setTarget(x * 4 + 450);
                pwm_set_duty_us(1,y);
                set_solenoid(button);

                if (IR_flag) {
                    IR_flag = 0;
                    transition_to_state(LIFE_LOST);

                    if (lives <= 0) {
                        transition_to_state(GAME_OVER);
                    }
                }

                score_update();
                break;
            }
            case LIFE_LOST:
                motorController_setTarget(x * 4 + 450);
                pwm_set_duty_us(1,y);

                if (new_message && button) {
                    set_solenoid(1);
                    transition_to_state(PLAYING);
                }
                break;

            case GAME_OVER:
                printf("GAME OVER\n");
                break;
        }

        if (new_message) {
            new_message = false;
        }

        // printf("%d %d %d ", x,y,button);
    }
}

void score_update(void){
    game_time = totalSeconds(time_now()) - game_start_time;
    
    CAN_MESSAGE msg_tx;
    msg_tx.id = 1;
    msg_tx.data_length = CANMSG_SCOREBOARD_DATA;
    msg_tx.data[0] = (uint8_t)(game_time & 0xFF);
    msg_tx.data[1] = (uint8_t)(game_time >> 8);
    msg_tx.data[2] = lives;

    if (!can_message_equal(&msg_tx, &last_msg_tx)) {
        can_send(&msg_tx,0);
        printf("Canbus transmit: %d %d\n", game_time, lives);
        can_copy_message(&msg_tx, &last_msg_tx);
    }
}

void score_init() {
    lives = 5;
    game_start_time = totalSeconds(time_now());
    last_msg_tx.id = 100;
}