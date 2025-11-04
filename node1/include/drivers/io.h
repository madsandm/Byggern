#ifndef IO_H
#define IO_H

#include <avr/io.h>
#include <utilities.h>
#include <util/delay.h>

uint8_t io_readTouchpad(uint8_t index);
uint8_t io_readSlider(uint8_t index);
uint8_t io_readJoystick(uint8_t index);
void io_readJoystickBuffer(uint8_t* buffer);
uint8_t io_readButtons(uint8_t index);
void io_writeLeds(uint8_t index, uint8_t value);


#endif // IO_H