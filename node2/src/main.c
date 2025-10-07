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
    uint32_t can_br = 0x123; // Example CAN bit rate setting
    
    SystemInit();
    configure_uart();
    can_init(0x123, 1, 1);
    
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer



    CAN_MESSAGE msg;
    while (1)
    {
        /* code */
        //printf("Hello World\n\r");
        can_receive(&msg, 1);
        printf("CAN message received: ID=%d, Length=%d, Data=", msg.id, msg.data_length);
        for (int i = 0; i < msg.data_length; i++)
        {
            printf(", %d", msg.data[i]);
        }
        printf("\n\r");
        for (int i = 0;i < 1000000;i++);
    }
    
}