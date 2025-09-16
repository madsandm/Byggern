#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

#define JOYSTICK_NEUTRAL_ZONE 10

typedef struct {
    const uint8_t xChannel;
    const uint8_t yChannel;

    const uint8_t minX;
    const uint8_t maxX;
    const uint8_t minY;
    const uint8_t maxY;
} IODevice;

IODevice joystick;
IODevice touchpad;

typedef struct {
    int8_t x;
    int8_t y;
} IPosition;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} DeviceDirection;

typedef struct {
    void (*calibrate)(IODevice device);
    IPosition (*getPosition)(IODevice device);
    DeviceDirection (*getDirection)(IODevice device);
} IIOGrid;


IIOGrid ioGrid;

#endif // JOYSTICK_H