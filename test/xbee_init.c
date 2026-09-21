#include <string.h>

#include "rti.h"
#include "sci.h"
#include "xbee/xbee.h"

// #define XBEE_GUARD_TIME 1000000 // Period of silence in microseconds before and after the command sequence

/* XBee API Initialization Routine */
void main(void)
{
    const char strOK[] = "OK\r";

    rtiInit();
    sciInit();

    /* Wait for the guard time before entering the command sequence */
    // rtiSetPeriod(rtiCOUNTER_BLOCK0, XBEE_GUARD_TIME);
    rtiStartCounter(rtiCOUNTER_BLOCK0);
    while (rtiGetCurrentTick(rtiCOMPARE0) != 0)
        ;

    /* Enter command mode */
    char buf1[] = "+++";
    sciSend(sciREG, 3, (uint8_t *)buf1);
    sciReceive(sciREG, 3, (uint8_t *)buf1); // Wait for "OK" response
    while (sciIsRxReady(sciREG) == 0)
        ;
    if (strncmp(buf1, strOK, 2) != 0)
    {
        while (1)
            ;
    }

    /* Enable API mode and configure UART (115200b/s, even parity, 1 stop bit) */
    char buf2[] = "ATAP1,WR,BD7,NB2,SB0,CN\r";
    sciSend(sciREG, sizeof(buf2) - 1, (uint8_t *)buf2);
    sciReceive(sciREG, 3, (uint8_t *)buf2); // Wait for "OK" response
    while (sciIsRxReady(sciREG) == 0)
        ;
    if (strncmp(buf2, strOK, 2) != 0)
    {
        while (1)
            ;
    }

    /* TODO: Use GPIO to display success/failure */
}
