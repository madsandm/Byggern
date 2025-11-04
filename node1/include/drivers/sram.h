#ifndef SRAM_H
#define SRAM_H

#include <stdint.h>

#define EXTERNAL_RAM_ADDRESS 0x1400
#define EXTERNAL_RAM_SIZE 0x0C00

volatile char* sram_data;

void sram_init();
void sram_test();
void* sram_malloc(uint16_t size);
void* sram_realloc(void* ptr, uint16_t size);


#endif // SRAM_H