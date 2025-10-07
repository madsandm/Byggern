#include "drivers/mcp2515.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <stdlib.h>

static uint8_t* mcp2515_read(uint8_t address, uint8_t size) {
    spi.slave_select(MCP2515_SELECT_PINS);

    spi.transmit(MCP2515_COMMAND_READ);
    spi.transmit(address);

    uint8_t* response = spi.receive_multi(size);

    spi.slave_deselect(MCP2515_SELECT_PINS);

    return response;
}

static void mcp2515_write(uint8_t address, const uint8_t* data, uint8_t size) {
    if (size <= 0) {
        return;
    }

    spi.slave_select(MCP2515_SELECT_PINS);

    spi.transmit(MCP2515_COMMAND_WRITE);
    spi.transmit(address);

    spi.transmit_multi(data, size);

    spi.slave_deselect(MCP2515_SELECT_PINS);
}

static void mcp2515_reset() {
    GPIO.initPin(&DDRB, MCP2515_CS, OUTPUT);
    spi.slave_deselect(MCP2515_SELECT_PINS);

    // Reset the controller and enter Configuration mode
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_RESET);
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

static void mcp2515_request_to_send(const uint8_t tx_buffers) {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_RTS | (tx_buffers & 0b00000111));
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

static uint8_t mcp2515_read_status() {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_READ_STATUS);
    uint8_t response = spi.receive();
    spi.slave_deselect(MCP2515_SELECT_PINS);
    return response;
}

static void mcp2515_bit_modify(const uint8_t address, uint8_t mask, const uint8_t data) {
    spi.slave_select(MCP2515_SELECT_PINS);
    spi.transmit(MCP2515_COMMAND_BIT_MODIFY);
    spi.transmit(address);
    spi.transmit(mask);
    spi.transmit(data);
    spi.slave_deselect(MCP2515_SELECT_PINS);
}

static void mcp2515_dump_memory() {
    
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

IMcp2515 mcp2515 = {
    .reset = mcp2515_reset,
    .read = mcp2515_read,
    .write = mcp2515_write,
    .read_status = mcp2515_read_status,
    .bit_modify = mcp2515_bit_modify,
    .request_to_send = mcp2515_request_to_send,
    .dump_memory = mcp2515_dump_memory
};