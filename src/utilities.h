#include <stdint.h>
#define INPUT 0
#define OUTPUT 1


void initializePin(volatile uint8_t* DD, uint8_t pin, uint8_t mode);
void togglePin(volatile uint8_t* port, uint8_t pin);
void blinky();