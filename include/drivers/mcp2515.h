#ifndef MCP2515_H
#define MCP2515_H

#include <avr/io.h>

#define MCP2515_CS_PORT &DDRA
#define MCP2515_CS 0
#define MCP2515_SELECT_PINS MCP2515_CS_PORT, MCP2515_CS

#define MCP2515_COMMAND_RESET        0b11000000
#define MCP2515_COMMAND_READ         0b00000011
#define MCP2515_COMMAND_WRITE        0b00000010
#define MCP2515_COMMAND_READ_STATUS  0b10100000
#define MCP2515_COMMAND_RX_STATUS    0b10110000
#define MCP2515_COMMAND_BIT_MODIFY   0b00000101

// The commands below have some bits that can be changed
#define MCP2515_COMMAND_READ_RX_BUF  0b10010000
#define MCP2515_COMMAND_LOAD_TX_BUF  0b01000000
#define MCP2515_COMMAND_RTS          0b10000000

// Modes
#define MCP2515_MODE_NORMAL     0b000
#define MCP2515_MODE_SLEEP      0b001
#define MCP2515_MODE_LOOPBACK   0b010
#define MCP2515_MODE_LISTENONLY 0b011
#define MCP2515_MODE_CONFIG     0b100

typedef struct {
    void (*reset)();
    uint8_t* (*read)(uint8_t address, uint8_t size);
    void (*write)(uint8_t address, const uint8_t* data, uint8_t size);
    void (*write_single)(uint8_t address, const uint8_t data);
    /**
     * Buffers where the three LSB are used to select TXB2, TXB1, TXB0
     */
    void (*request_to_send)(const uint8_t tx_buffers);
    uint8_t (*read_status)();
    void (*bit_modify)(const uint8_t address, uint8_t mask, const uint8_t data);
} IMcp2515;

IMcp2515 mcp2515;

#endif // MCP2515_H