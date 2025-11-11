#ifndef UTILITIES_H
#define UTILITIES_H

#include "sam.h"
#include "can_controller.h"
#include <stdbool.h>

extern uint32_t game_start_time;
extern uint32_t game_time;
extern uint32_t lives;
extern uint32_t game_freeze;
extern uint32_t game_freeze_time;
extern CAN_MESSAGE last_msg_tx;

typedef enum {
    IDLE,
    PLAYING,
    LIFE_LOST,
    GAME_OVER
} GAME_STATE;

void transition_to_state(GAME_STATE state);

uint32_t can_joystick_to_us();
void score_update(void);
void score_init();



#endif