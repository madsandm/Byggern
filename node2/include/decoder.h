#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "sam.h"

#define DECODER_WPKEY (0x54494D << 8) // TIM in ASCII
#define DECODER_MAXFILT 10 // value from 1 to 63

void decoder_init();
int decoder_read();
void decoder_setnullpoint();

#endif // ENCODER_H