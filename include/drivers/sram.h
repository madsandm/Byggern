#ifndef SRAM_H
#define SRAM_H

#include <stdint.h>
#include <stdlib.h>

#define EXTERNAL_RAM_ADDRESS 0x1400
#define EXTERNAL_RAM_SIZE 0x0C00

typedef struct {
    volatile char* data;
    const uint16_t size;
    void (*init)();
    void (*test)();
    void* (*malloc)(const size_t size);
    void* (*realloc)(const void* ptr, const size_t size);
} ISRAM;

ISRAM sram;

#endif // SRAM_H
