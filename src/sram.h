#ifndef SRAM_H
#define SRAM_H

#include <stdint.h>

#define EXTERNAL_RAM_ADDRESS 0x0500
#define EXTERNAL_RAM_SIZE 0x0800

void SRAM_test();

typedef struct {
    volatile char* data;
    const uint16_t size;
    void (*init)();
} ISRAM;

ISRAM sram;

#endif // SRAM_H