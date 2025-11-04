#include "drivers/sram.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

static void SRAM_init() {
    // Enable external memory
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM0);
}

uint16_t sram_heap_ptr = EXTERNAL_RAM_ADDRESS + 0x800; // Remove space used for oled
static void* SRAM_malloc(uint16_t size) {
    if (size + sram_heap_ptr > EXTERNAL_RAM_ADDRESS + EXTERNAL_RAM_SIZE) {
        return NULL;
    }
    uint16_t ret = sram_heap_ptr;
    sram_heap_ptr += size;
    return ret;
}

static void* SRAM_realloc(uint8_t* ptr, uint16_t size) {
    uint8_t* newSpace = SRAM_malloc(size);
    for (uint16_t i = 0; i < size; i++) {
        newSpace[i] = ptr[i];
    }
    return newSpace;
}


ISRAM sram = {
    .data = (char*)EXTERNAL_RAM_ADDRESS,
    .size = EXTERNAL_RAM_SIZE,
    .init = SRAM_init,
    .malloc = SRAM_malloc,
    .realloc = SRAM_realloc
};