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
#include "can.h"

int main()
{
    CanInit init = {
        .phase2 = 4,  // Phase 2 segment
        .propag = 4,  // Propagation time segment
        .phase1 = 4,  // Phase 1 segment
        .sjw = 4,     // Synchronization jump width
        .brp = 8,     // Baud rate prescaler
        .smp = 8      // Sampling mode
    };
    
    SystemInit();
    configure_uart();
    //can_init(init, 0);
    
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer



    CanMsg msg;
    while (1)
    {
        /* code */
        //printf("Hello World\n\r");
        //can_rx(&msg);
        printf("CAN message received: ID=%d, Length=%d, Data=", msg.id, msg.length);
        for (int i = 0; i < msg.length; i++)
        {
            printf(", %d", msg.byte[i]);
        }
        printf("\n\r");
        for (int i = 0;i < 1000000;i++);
    }
    
}