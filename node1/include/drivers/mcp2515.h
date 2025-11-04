#ifndef MCP2515_H
#define MCP2515_H

#include <avr/io.h>

#define MCP2515_CS_PORT &PORTB
#define MCP2515_CS PB2
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

// Registers
#define TXRTSCTRL 0x0d
#define CANCTRL 0x0f
#define CANINTE 0x2b
#define CANINTF 0x2c
#define BFPCTRL 0x0c
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2a

// CANINTE bit shifts
#define RX0IE 0
#define RX1IE 1
#define TX0IE 2
#define TX1IE 3
#define TX2IE 4

// CANINTF bit shifts
#define RX0IF 0
#define RX1IF 1
#define TX0IF 2
#define TX1IF 3
#define TX2IF 4

// BFPCTRL bit shifts
#define B0BFM 0
#define B1BFM 1
#define B0BFE 2
#define B1BFE 3
#define B0BFS 4
#define B1BFS 5

// CNF Registers
#define CNF1_SJW 6
#define CNF1_BRP 0

#define CNF2_BTLMODE 7
#define CNF2_SAM 6
#define CNF2_PHSEG1 3
#define CNF2_PRSEG 0

#define CNF3_SOF 7
#define CNF3_WAKFIL 6
#define CNF3_PHSEG2 0


// Receive registers
#define RXB0 0b01100000
#define RXB1 0b01110000

// Transmit registers
#define TXB0 0b00110000
#define TXB1 0b01000000
#define TXB2 0b01010000

#define TXBnCTRL 0
#define TXBnSIDH 0b0001 // Standard identifier high
#define TXBnSIDL 0b0010 // Standard identifier low
#define TXBnDLC  0b0101 // Data length code
#define TXBnD0   0b0110 // Transmit buffer data byte 0

void mcp2515_reset();
void mcp2515_read_into(uint8_t* dst, uint8_t address, uint8_t size);
void mcp2515_write(uint8_t address, const uint8_t* data, uint8_t size);
void mcp2515_requestToSend(const uint8_t tx_buffers);
uint8_t mcp2515_readStatus();
void mcp2515_bitModify(const uint8_t address, uint8_t mask, const uint8_t data);
void mcp2515_dumpMemory();

#endif // MCP2515_H