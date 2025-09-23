#ifndef UTILITIES_H
#define UTILITIES_H

#define F_CPU 4915200UL
#include <avr/delay.h>
#include <stdint.h>

void blinky(uint8_t times);
void uart_led_command();

void test_latch();

#endif // UTILITIES_H
