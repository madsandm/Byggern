#include "drivers/mcp2515.h"
#include "drivers/spi.h"
#include "drivers/gpio.h"
#include <stdlib.h>

void mcp2515_read_into(uint8_t* dst, uint8_t address, uint8_t size) {
    spi_slave_select(MCP2515_SELECT_PINS);
    spi_transmit(MCP2515_COMMAND_READ);
    spi_transmit(address);
    spi_receive_multi_into(dst, size);
    spi_slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_write(uint8_t address, const uint8_t* data, uint8_t size) {
    if (size <= 0) {
        return;
    }

    spi_slave_select(MCP2515_SELECT_PINS);

    spi_transmit(MCP2515_COMMAND_WRITE);
    spi_transmit(address);

    spi_transmit_multi(data, size);

    spi_slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_reset() {
    gpio_initPin(&DDRB, MCP2515_CS, OUTPUT);
    spi_slave_deselect(MCP2515_SELECT_PINS);

    // Reset the controller and enter Configuration mode
    spi_slave_select(MCP2515_SELECT_PINS);
    spi_transmit(MCP2515_COMMAND_RESET);
    spi_slave_deselect(MCP2515_SELECT_PINS);
}

void mcp2515_requestToSend(const uint8_t tx_buffers) {
    spi_slave_select(MCP2515_SELECT_PINS);
    spi_transmit(MCP2515_COMMAND_RTS | (tx_buffers & 0b00000111));
    spi_slave_deselect(MCP2515_SELECT_PINS);
}

uint8_t mcp2515_readStatus() {
    spi_slave_select(MCP2515_SELECT_PINS);
    spi_transmit(MCP2515_COMMAND_READ_STATUS);
    uint8_t response = spi_receive();
    spi_slave_deselect(MCP2515_SELECT_PINS);
    return response;
}

void mcp2515_bitModify(const uint8_t address, uint8_t mask, const uint8_t data) {
    spi_slave_select(MCP2515_SELECT_PINS);
    spi_transmit(MCP2515_COMMAND_BIT_MODIFY);
    spi_transmit(address);
    spi_transmit(mask);
    spi_transmit(data);
    spi_slave_deselect(MCP2515_SELECT_PINS);
}
