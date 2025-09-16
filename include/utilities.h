#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdint.h>

void blinky(uint8_t times);
void uart_led_command();

void test_latch();