#ifndef SRAM_H
#define SRAM_H

#define EXTERNAL_RAM_ADDRESS 0x1800

void SRAM_test();

typedef struct {
    char* data;
    void (*init)();
} ISRAM;

ISRAM sram;

#endif // SRAM_H