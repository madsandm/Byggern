#include "sram.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

static void SRAM_init() {
    // Enable external SRAM
    MCUCR |= (1 << SRE);
}

ISRAM sram = {
    .data = (char*)EXTERNAL_RAM_ADDRESS,
    .size = EXTERNAL_RAM_SIZE,
    .init = SRAM_init
};

void SRAM_test() {
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
            printf("Write phase error: EXTERNAL_RAM[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }

    srand(seed);
    for (uint16_t i = 0; i < sram.size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = sram.data[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: EXTERAL_RAM[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with %d write errors and %d retrieval errors\n", write_errors, retrieval_errors);
}