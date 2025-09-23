#ifndef IO_H
#define IO_H

#include <avr/io.h>
#include <util/delay.h>


typedef struct {
    uint8_t (*read_touchpad)(uint8_t index);
    uint8_t (*read_slider)(uint8_t index);
    uint8_t (*read_joystick)(uint8_t index);
    uint8_t (*read_buttons)(uint8_t index);
    void (*write_leds)(uint8_t index, uint8_t value);

} IIO;

IIO io;





#endif // IO_H