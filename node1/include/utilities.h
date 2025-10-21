#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>
#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdint.h>

void blinky(uint8_t times);
void uart_led_command();

void test_latch();

void send_data(FILE* stream, const char* data, size_t size);

void etch_a_sketch();

void pong();

void can_joystick();

#endif // UTILITIES_H
