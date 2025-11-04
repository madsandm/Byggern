#include "drivers/sram.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

volatile char* sram_data = (volatile char*)EXTERNAL_RAM_ADDRESS;

void sram_init() {
    // Enable external memory
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM0);
}


// void sram_test() {
//     uint16_t write_errors = 0;
//     uint16_t retrieval_errors = 0;
//     printf("\n\nStarting SRAM test...\n");

//     uint16_t seed = rand();
//     srand(seed);
//     for (uint16_t i = 0; i < EXTERNAL_RAM_SIZE; i++) {
//         uint8_t some_value = rand();
//         sram_data[i] = some_value;
//         uint8_t retreived_value = sram_data[i];
//         if (retreived_value != some_value) {
//             printf("Write err: RAM[%4d] = %02X (expected %02X)\n", i, retreived_value, some_value);
//             write_errors++;
//         }
//     }

//     srand(seed);
//     for (uint16_t i = 0; i < EXTERNAL_RAM_SIZE; i++) {
//         uint8_t some_value = rand();
//         uint8_t retreived_value = sram_data[i];
//         if (retreived_value != some_value) {
//             printf("Retrieval err: RAM[%4d] = %02X (expected %02X)\n", i, retreived_value, some_value);
//             retrieval_errors++;
//         }
//     }

//     printf("SRAM test completed with %d write errors and %d retrieval errors\n", write_errors, retrieval_errors);
// }

uint16_t sram_heap_ptr = EXTERNAL_RAM_ADDRESS + 0x800; // Remove space used for oled
void* sram_malloc(uint16_t size) {
    if (size + sram_heap_ptr > EXTERNAL_RAM_ADDRESS + EXTERNAL_RAM_SIZE) {
        return NULL;
    }
    uint16_t ret = sram_heap_ptr;
    sram_heap_ptr += size;
    return (void*)ret;
}

void* sram_realloc(void* ptr, uint16_t size) {
    uint8_t* newSpace = sram_malloc(size);
    for (uint16_t i = 0; i < size; i++) {
        newSpace[i] = ((uint8_t*)ptr)[i];
    }
    return newSpace;
}
