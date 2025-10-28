#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "uart.h"
#include "can_controller.h"
#include "pwm.h"
#include "motor_driver.h"
#include "utilities.h"
#include "decoder.h"
#include "controller.h"


int main()
{
    uint32_t can_br = 0 << 24 | 41 << 16 | 0 << 12 | 5 << 8 | 5 << 4 | 2; // CAN bit rate setting
    
    SystemInit();
    configure_uart();
    pwm_init();
    pwm_pin_setup();
    servo_init();
    motor_driver_init();
    
    motorController_init();
    decoder_init();

    uint8_t status = can_init(can_br, 1, 2);
    if (status) {
        printf("CAN initialization failed\n\r");
    } else {
        printf("CAN initialized successfully\n\r");
    }

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    can_joystick_to_us();
    
    while (1)
    {
        /* code */
        CAN_MESSAGE msg_rx;
        //printf("Hello World\n\r");
        if (can_receive(&msg_rx, 0) == 0) {
            printf("ID: %d, Size: %d, Data: ", msg_rx.id, msg_rx.data_length);
            for (int i = 0; i < msg_rx.data_length; i++) {
                printf("%d ", (unsigned char)msg_rx.data[i]);
            }
            printf("\n\r");
        } else {
            //printf("No CAN message received\n\r");
        }
    }

    // while (1)
    // {
    //     /* code */
    //     CAN_MESSAGE msg_rx;
    //     //printf("Hello World\n\r");
    //     if (can_receive(&msg_rx, 0) == 0) {
    //         printf("ID: %d, Size: %d, Data: ", msg_rx.id, msg_rx.data_length);
    //         for (int i = 0; i < msg_rx.data_length; i++) {
    //             printf("%d ", (unsigned char)msg_rx.data[i]);
    //         }
    //         printf("\n\r");
    //     } else {
    //         //printf("No CAN message received\n\r");
    //     }
    // }
    
}