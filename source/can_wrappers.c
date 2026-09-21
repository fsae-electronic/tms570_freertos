#include "can_wrappers.h"

void canAbortTransmission(canBASE_t *node, uint32_t messageBox)
{
    /* Clear TxRqst */
    node->IF1CMD = 0x04U;              // Set Direction to Write, Control bits active, Clear TxRqst
    node->IF1NO = (uint8_t)messageBox; // Point to the stuck mailbox

    while ((node->IF1STAT & 0x80U) == 0x80U)
    {
        /* Wait for the busy bit to clear to ensure the hardware processes the abort */
    }
}

void canHandleBusOffRecovery(canBASE_t *node)
{
    uint8_t i = 1;

    /* The DCAN module sets the Init bit automatically to enter Bus-off. To trigger a
     * recovery sequence, the Init bit must be cleared.
     */

    /* Forcefully clear pending Tx Requests so they don't corrupt recovery */
    for (; i < 64; i++)
    {
        canAbortTransmission(node, i); // Abort all mailboxes
    }

    /* Request recovery by clearing the hardware Init bit */
    node->CTL &= ~(0x00000001U);

    /* The DCAN module will now wait internally for 129 occurrences of 11 consecutive
     * recessive bits (idle) on the bus before transitioning back to Error Active.
     */
}

uint8_t canGetErrorCount(canBASE_t *node)
{
    uint32_t rawCounters = node->EERC; // Raw precise tracking via the Error Counter Register
    uint8_t transmitErrorCount = (uint8_t)((rawCounters & 0x0000FF00U) >> 8U);

    return transmitErrorCount;
}

uint64_t canIsTxMessagesPending(canBASE_t *node, uint64_t messageBoxes)
{
    uint64_t flag = 0;
    uint8_t regIndex = 0;
    uint32_t bitIndexes = 0;

    /* Check each 32-bit register for pending transmissions */
    for (; regIndex < 2; regIndex += 32)
    {
        bitIndexes = (messageBoxes >> regIndex) & 0xFFFFFFFFU;
        flag |= ((uint64_t)(node->TXRQx[regIndex] & bitIndexes)) << regIndex;
    }

    return flag;
}
