#include "drivers/canbus.h"
#include "drivers/mcp2515.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>

bool packet_available = false;

static void canbus_init() {
    mcp2515.reset();

    // printf("Canbus setup:\n");
    // printf("PropSeg = %d\n", CANBUS_PROPSEG);
    // printf("PS1 = %d\n", CANBUS_PS1);
    // printf("PS2 = %d\n", CANBUS_PS2);
    // printf("BRP = %d\n", CANBUS_BRP);
    // printf("SJW = %d\n", CANBUS_BRP);
    // printf("NBT = %d\n", CANBUS_NBT);

    const uint8_t PHSEG2 = CANBUS_PS2 - 1;
    const uint8_t PRSEG = CANBUS_PROPSEG - 1;
    const uint8_t PHSEG1 = CANBUS_PS1 - 1;
    const uint8_t BRP = CANBUS_BRP - 1;
    const uint8_t SJW = CANBUS_SJW - 1;

    const uint8_t cnf1 = ((SJW << CNF1_SJW) | (BRP << CNF1_BRP));
    const uint8_t cnf2 = ((1 << CNF2_BTLMODE) | (PHSEG1 << CNF2_PHSEG1) | (PRSEG << CNF2_PRSEG));
    const uint8_t cnf3 = (PHSEG2 << CNF3_PHSEG2);

    // Set CAN bus bit timings
    mcp2515.bit_modify(CNF1, 0xff, cnf1);
    mcp2515.bit_modify(CNF2, 0xff, cnf2);
    mcp2515.bit_modify(CNF3, 0xff, cnf3);

    // Enable interrupts
    mcp2515.bit_modify(CANINTE, 1 << RX0IE, 0xff);

    // setup receive criteria
    mcp2515.bit_modify(RXB0 | TXBnCTRL, 0b01100100, 0b01100000); // Accept all, without rollover

    // Enter desired mode
    //mcp2515.bit_modify(CANCTRL, 7 << 5, MCP2515_MODE_LOOPBACK << 5);
    mcp2515.bit_modify(CANCTRL, 7 << 5, MCP2515_MODE_NORMAL << 5);

    // Enable external interrupt on INT0 (PD2)
    MCUCR |= (1 << ISC01); // Trigger INT0 on falling edge
    GICR  |= (1 << INT0); // Enable INT0
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

    CanbusPacket ret = {
        .id = id,
        .size = size,
    };

    for (uint8_t i = 0; i < size; i++) {
        ret.data[i] = frame[5 + i];
    }
    free(frame);

    return ret;
}

static CanbusPacket canbus_create_packet_from_string(uint16_t id, char* str) {
    CanbusPacket ret = {
        .id = id,
        .size = 8
    };

    bool endFound = false;
    for (uint8_t i = 0; i < 8; i++) {
        if (str[i] == 0) {
            ret.size = i;
            endFound = true;
            break;
        }
        ret.data[i] = str[i];
    }

    if (!endFound && str[8] != 0) {
        printf("String is too long for a canbus packet");
    }

    return ret;
}

static CanbusPacket canbus_create_packet(uint16_t id, uint8_t* data, uint8_t size) {
    if (size > 8) {
        printf("Canbus data to send is too large.");
    }
    
    CanbusPacket ret = {
        .id = id,
        .size = size
    };

    for (uint8_t i = 0; i < size; i++) {
        ret.data[i] = data[i];
    }

    return ret;
}

ISR(INT0_vect) {
    packet_available = true;
}

ICanbus canbus = {
    .init = canbus_init,
    .transmit = canbus_transmit,
    .receive = canbus_receive,
    .create_packet_from_string = canbus_create_packet_from_string,
    .create_packet = canbus_create_packet
};