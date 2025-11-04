#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * CANBUS BIT TIMINGS
 * TQ = Time Quanta (Minimum timing used in the protocol)
 * NBT = Normal Bit Time = 16 TQ (At least I think 16 TQ is a good choice...)
 * SyncSeg = syncronises the nodes on the bus, during this time the rising edge happens = 1 TQ
 * PropSeg = Time to compensate for delays between nodes = (1 -> 8) TQ
 * PS1 = Phase segment 1 = (1 -> 8) TQ
 * PS2 = Phase segment 2 = (2 -> 8) TQ
 * NBT = SyncSeg + PropSeg + PS1 + PS2
 * The sampeling happens between PS1 and PS2
 * 
 * BRP = Bit rate prescaler
 * IPT = Information Processing time (Takes 2 TQ, starts right after PS1)
 * 
 * SJW = Synchronization Jump Width (How much PS1 can be ngthened, and PS2 can be shortened do syncronize with other nodes.)
 *      I think this happens to shorten or lengthen NBT with the value of SJW, so the clocks will syncronize.
 * 
 * CONSTRAINTS
 * PropSeg + PS1 >= PS2
 * PropSeg + PS1 >= T_delay (I think this is the delay between the nodes)
 * PS2 > SJW
 * 
 * Equation 5-2 from the datasheet
 * TQ = 2 * BRP / F_OSC
 * where F_OSC is the clock frequency
 */

 // NOTE: The definitions above the the values for the wanted result, NOT the values sent to the registers
#define CANBUS_F_OSC 16000000UL
#define CANBUS_FREQ 125000UL
#define CANBUS_PROPSEG 5
#define CANBUS_PS1 5
#define CANBUS_PS2 5
#define CANBUS_T_Q_100NS 5
#define CANBUS_SJW 1
#define CANBUS_NBT 1 + CANBUS_PROPSEG + CANBUS_PS1 + CANBUS_PS2
#define CANBUS_BRP 2 //((uint8_t)(CANBUS_F_OSC * CANBUS_T_Q_100NS / 2 / 10000000UL))

typedef struct CanbusPacket {
    uint16_t id;
    uint8_t size;
    uint8_t data[8];
} CanbusPacket;

extern bool packet_available;

typedef struct {
    void (*init)();
    void (*transmit)(CanbusPacket packet);
    CanbusPacket (*receive)();
    CanbusPacket (*create_packet_from_string)(uint16_t id, char* str);
    CanbusPacket (*create_packet)(uint16_t id, uint8_t* data, uint8_t size);
} ICanbus;

ICanbus canbus;

#endif // CANBUS_H