/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 
#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
#include "stdint.h"

volatile extern uint8_t RX_MB1_FLAG;
volatile extern uint8_t RX_MB2_FLAG;

void CAN0_Handler       ( void );



#endif /* CAN_INTERRUPT_H_ */