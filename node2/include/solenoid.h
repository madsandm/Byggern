#ifndef SOLENOID_H
#define SOLENOID_H

#include "sam.h"

void solenoid_init(void);
void set_solenoid(uint8_t state);
#endif // SOLENOID_H