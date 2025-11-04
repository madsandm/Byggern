#include "drivers/joystick.h"
#include "drivers/adc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

IODevice joystick = {
    .xChannel = 0,
    .yChannel = 1,
    .minX = 71,
    .maxX = 248,
    .minY = 65,
    .maxY = 252
};

IODevice touchpad = {
    .xChannel = 2,
    .yChannel = 3,
    .minX = 2,
    .maxX = 236,
    .minY = 2,
    .maxY = 236
};

void IODevice_calibrate(IODevice device) {
    printf("Starting Grid Device calibration...\n");

    uint8_t x_min = 255, x_max = 0;
    uint8_t y_min = 255, y_max = 0;

    while (true) {
        for(uint32_t i = 0; i < 40000; i++) {
            adc_read();

            uint8_t x_value = adc_getChannel(device.xChannel);
            uint8_t y_value = adc_getChannel(device.yChannel);

            if(x_value < x_min) x_min = x_value;
            if(x_value > x_max) x_max = x_value;
            if(y_value < y_min) y_min = y_value;
            if(y_value > y_max) y_max = y_value;
        }

        printf("Device X Range: %u - %u\n", x_min, x_max);
        printf("Device Y Range: %u - %u\n", y_min, y_max);

    }
}

IPosition IODevice_getPosition(IODevice device) {
    IPosition pos;

    adc_read();
    int32_t x_value = (int32_t)adc_getChannel(device.xChannel);
    int32_t y_value = (int32_t)adc_getChannel(device.yChannel);

    pos.x = (int8_t)(
        (x_value - device.minX) * 200 / (device.maxX - device.minX) - 100
    );
    pos.y = (int8_t)(
        (y_value - device.minY) * 200 / (device.maxY - device.minY) - 100
    );

    return pos; // returns position in range -100 to 100
}

DeviceDirection IODevice_getDirection(IODevice device) {
    IPosition pos = IODevice_getPosition(device);

    if (abs(pos.x) < JOYSTICK_NEUTRAL_ZONE && abs(pos.y) < JOYSTICK_NEUTRAL_ZONE) {
        return NEUTRAL;
    } else if (abs(pos.x) > abs(pos.y)) {
        return (pos.x > 0) ? RIGHT : LEFT;
    } else {
        return (pos.y > 0) ? UP : DOWN;
    }
}
