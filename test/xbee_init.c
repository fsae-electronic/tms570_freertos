#include <string.h>

#include "rti.h"
#include "sci.h"
#include "xbee/xbee.h"

#define CMD_MODE_1
// #define CMD_MODE_2

/* RTI Compare silence periods:
 * - Counter 1 Compare 0: 1s
 * - Counter 1 Compare 1: 6s
 */
#define RTI_COUNTER_BLOCK1_CLK 1000000U // MHz
#define RTI_COMPARE0_PERIOD 1000U       // ms
#define RTI_COMPARE1_PERIOD 6000U       // ms

#define RTI_UPDATE_COMPARE0 (RTI_COUNTER_BLOCK1_CLK * RTI_COMPARE0_PERIOD / 1000U)
#define RTI_UPDATE_COMPARE1 (RTI_COUNTER_BLOCK1_CLK * RTI_COMPARE1_PERIOD / 1000U)

/* XBee API Initialization Routine */
void main(void)
{
    const char strOK[] = "OK\r";
    char buf[3] = {0};

    rtiInit();
    sciInit();

    rtiSetPeriod(rtiCOMPARE0, RTI_UPDATE_COMPARE0); // 1s
    rtiSetPeriod(rtiCOMPARE1, RTI_UPDATE_COMPARE1); // 6s

    while ((scilinREG->FLR & 0x04U) == 0U)
    {
    } // Wait until the SCI is idle (TX EMPTY flag is set)
    sciReceive(scilinREG, 3, (uint8_t *)buf); // Prepare to receive "OK" response

#if defined(CMD_MODE_1)
    /* Wait for the guard time before entering the command sequence */
    rtiStopCounter(rtiCOUNTER_BLOCK1);
    rtiResetCounter(rtiCOUNTER_BLOCK1);
    rtiStartCounter(rtiCOUNTER_BLOCK1);
    while (rtiGetCurrentTick(rtiCOMPARE0) != 0);

    /* Enter command mode */
    char buf1[] = "+++";
    sciSend(scilinREG, 3, (uint8_t *)buf1);
    // while (sciIsTxReady(scilinREG) == 0);
#elif defined(CMD_MODE_2)
    /* Send serial break */
    scilinREG->PIO0 &= ~((uint32_t)1U << 2); // TX FUNC: GPIO
    scilinREG->PIO1 |= ((uint32_t)1U << 2);  // TX DIR: output
    scilinREG->PIO3 |= ((uint32_t)1U << 2);  // TX OUT: logic high
    scilinREG->PIO4 |= ((uint32_t)1U << 2);  // TX SET: asserted

    rtiStopCounter(rtiCOUNTER_BLOCK1);
    rtiResetCounter(rtiCOUNTER_BLOCK1);
    rtiStartCounter(rtiCOUNTER_BLOCK1);
    while (rtiGetCurrentTick(rtiCOMPARE1) != 0);

    scilinREG->PIO0 |= ((uint32)1U << 2); // TX FUNC: SCI
#endif

    while (sciIsRxReady(scilinREG) == 0);
    if (strncmp(buf, strOK, 3) != 0)
        while (1);

    buf[0] = buf[1] = buf[2] = 0;
    sciReceive(scilinREG, 3, (uint8_t *)buf); // Setup next reception

    /* Enable API mode and configure UART (115200b/s, even parity, 1 stop bit) */
    char buf2[] = "ATAP1,WR,BD7,NB2,SB0,CN\r";
    sciSend(scilinREG, sizeof(buf2) - 1, (uint8_t *)buf2);
    // while (sciIsTxReady(scilinREG) == 0);

    while (sciIsRxReady(scilinREG) == 0);
    if (strncmp(buf, strOK, 3) != 0)
        while (1);
}
