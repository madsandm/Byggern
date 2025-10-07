#include "drivers/canbus.h"
#include "drivers/mcp2515.h"
#include <stdlib.h>


static void canbus_init() {
    mcp2515.reset();

    // Set CAN bus bit timings
    mcp2515.write(CNF3, 0x05, 1);
    mcp2515.write(CNF2, 0xa4, 1);
    mcp2515.write(CNF1, 0x81, 1);

    // Enable interrupts
    mcp2515.bit_modify(CANINTE, 1 << RX0IE, 0xff);

    // setup receive criteria
    mcp2515.bit_modify(RXB0 | TXBnCTRL, 0b01100100, 0b01100000); // Accept all, without rollover

    // Enter desired mode
    //mcp2515.bit_modify(CANCTRL, 7 << 5, MCP2515_MODE_LOOPBACK << 5);
    mcp2515.bit_modify(CANCTRL, 7 << 5, MCP2515_MODE_NORMAL << 5);
}

static void canbus_transmit(CanbusPacket packet) {
    uint8_t stdIDHigh = (packet.id >> 3) & 0xFF;
    uint8_t stdIDLow = (packet.id & 0b111) << 5;

    // TODO: handle messages > 8

    uint8_t packetSize = packet.size & 0x0f;

    uint8_t header[] = {
        stdIDHigh,
        stdIDLow,
        0, // Two extended id bytes
        0,
        packetSize
    };

    mcp2515.write(TXB0 | TXBnSIDH, header, sizeof(header));

    mcp2515.write(TXB0 | TXBnD0, packet.data, packetSize);

    mcp2515.request_to_send(0b001);
}

static CanbusPacket canbus_receive() {
    uint8_t* frame = mcp2515.read(RXB0 | TXBnSIDH, 13);

    uint16_t id = frame[0];
    id = (id << 3) | (frame[1] >> 5);

    uint8_t size = frame[4] & 0x0F;
    uint8_t* data = malloc(size);
    for (uint8_t i = 0; i < size; i++) {
        data[i] = frame[5 + i];
    }

    free(frame);

    return (CanbusPacket){
        .id = id,
        .data = data,
        .size = size
    };
}

ICanbus canbus = {
    .init = canbus_init,
    .transmit = canbus_transmit,
    .receive = canbus_receive
};