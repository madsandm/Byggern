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

int main()
{
    uint32_t can_br = 0x00252566; // CAN bit rate setting
    
    SystemInit();
    configure_uart();
    uint8_t status = can_init(can_br, 1, 2);
    if (status) {
        printf("CAN initialization failed\n\r");
    } else {
        printf("CAN initialized successfully\n\r");
    }

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer



    CAN_MESSAGE msg;
    while (1)
    {
        /* code */
        //printf("Hello World\n\r");
        //can_receive(&msg, 0);
        printf("CAN message received: ID=%d, Length=%d, Data=", msg.id, msg.data_length);
        for (int i = 0; i < msg.data_length; i++)
        {
            printf("%c", msg.data[i]);
        }
        printf("\n\r");
        for (int i = 0;i < 100000;i++);
        msg.id = 4;
        msg.data_length = 2;
        msg.data[0] = 'h';
        msg.data[1] = 'e';
        can_send(&msg, 0);
        for (int i = 0;i < 100000;i++);
    }
    
}