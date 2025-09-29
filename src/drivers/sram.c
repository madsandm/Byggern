#include "drivers/sram.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

static void SRAM_init() {
    // Enable external memory
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM0);
}


static void SRAM_test() {
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("\n\nStarting SRAM test...\n");

    uint16_t seed = rand();
    srand(seed);
    for (uint16_t i = 0; i < sram.size; i++) {
        uint8_t some_value = rand();
        sram.data[i] = some_value;
        uint8_t retreived_value = sram.data[i];
        if (retreived_value != some_value) {
            printf("Write err: RAM[%4d] = %02X (expected %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }

    srand(seed);
    for (uint16_t i = 0; i < sram.size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = sram.data[i];
        if (retreived_value != some_value) {
            printf("Retrieval err: RAM[%4d] = %02X (expected %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with %d write errors and %d retrieval errors\n", write_errors, retrieval_errors);
}

size_t lastAllocation = EXTERNAL_RAM_ADDRESS;
static void* SRAM_malloc(const size_t size) {
    if (lastAllocation + size > EXTERNAL_RAM_ADDRESS + EXTERNAL_RAM_SIZE) {
        printf("FAILED TO ALLOCATE SRAM\n");
        return NULL;
    }
    size_t lastAllocation;
    lastAllocation += size;
    return (void*)lastAllocation;
}

static void* SRAM_realloc(const void* ptr, const size_t size) {
    void* newAllocation = SRAM_malloc(size);
    char* dest = (char*)newAllocation;
    char* src = (char*)ptr;

    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }

    return newAllocation;
}

ISRAM sram = {
    .data = (char*)EXTERNAL_RAM_ADDRESS,
    .size = EXTERNAL_RAM_SIZE,
    .init = SRAM_init,
    .test = SRAM_test,
    .malloc = SRAM_malloc,
    .realloc = SRAM_realloc
};
