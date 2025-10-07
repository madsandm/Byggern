#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>

typedef struct CanbusPacket {
    uint16_t id;
    uint8_t* data;
    uint8_t size;
} CanbusPacket;

typedef struct {
    void (*init)();
    void (*transmit)(CanbusPacket packet);
    CanbusPacket (*receive)();
} ICanbus;

ICanbus canbus;

#endif // CANBUS_H