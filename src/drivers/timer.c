#include "drivers/timer.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "drivers/uart.h"

void delay(uint32_t milliseconds) {
    for (volatile uint32_t i = 0; i < milliseconds * 70; i++) {
        __asm__ __volatile__("nop");
    }
}
