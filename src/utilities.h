#include <stdint.h>
#define INPUT 0
#define OUTPUT 1


void initialize_pin(volatile uint8_t* DD, uint8_t pin, uint8_t mode);
void toggle_pin(volatile uint8_t* port, uint8_t pin);
void set_pin(volatile uint8_t* port, uint8_t pin);
void clear_pin(volatile uint8_t* port, uint8_t pin);
void blinky(uint8_t times);
void uart_led_command();