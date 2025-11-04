#include "drivers/mcp2515.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <stdlib.h>

uint8_t* mcp2515_read(uint8_t address, uint8_t size) {
    spi.slave_select(MCP2515_SELECT_PINS);

    spi.transmit(MCP2515_COMMAND_READ);
    spi.transmit(address);

    uint8_t* response = spi.receive_multi(size);

    spi.slave_deselect(MCP2515_SELECT_PINS);

    return response;
}

void mcp2515_write(uint8_t address, const uint8_t* data, uint8_t size) {
    if (size <= 0) {
        return;
    }

    spi.slave_select(MCP2515_SELECT_PINS);

    spi.transmit(MCP2515_COMMAND_WRITE);
    spi.transmit(address);

    spi.transmit_multi(data, size);

    spi.slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_reset() {
    gpio_initPin(&DDRB, MCP2515_CS, OUTPUT);
    spi.slave_deselect(MCP2515_SELECT_PINS);

    // Reset the controller and enter Configuration mode
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_RESET);
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_requestToSend(const uint8_t tx_buffers) {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_RTS | (tx_buffers & 0b00000111));
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

uint8_t mcp2515_readStatus() {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_READ_STATUS);
    uint8_t response = spi.receive();
    spi.slave_deselect(MCP2515_SELECT_PINS);
    return response;
}

void mcp2515_bitModify(const uint8_t address, uint8_t mask, const uint8_t data) {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_BIT_MODIFY);
    spi.transmit(address);
    spi.transmit(mask);
    spi.transmit(data);
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_dumpMemory() {
    
    printf("MCP2515 memory dump\n");
    for (char i = 0; i < 8; i++) {
        uint8_t* data = mcp2515_read(i * 8, 8);
        for (char j = 0; j < 8; j++) {
            printf("%x ", data[j]);
        }
        free(data);
        printf("\n");
    }
}
