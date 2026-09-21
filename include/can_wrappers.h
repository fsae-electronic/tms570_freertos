#ifndef _CAN_WRAPPERS_H_
#define _CAN_WRAPPERS_H_

#include "can.h"

/**
 * @brief Aborts a CAN transmission in a specific message box
 * @param node The CAN module instance
 * @param messageBox The message box to abort
 */
void canAbortTransmission(canBASE_t *node, uint32_t messageBox);

/**
 * @brief Handles CAN bus-off recovery protocol
 * @param node The CAN module instance.
 */
void canHandleBusOffRecovery(canBASE_t *node);

/**
 * @brief Retrieves the current CAN transmission error count
 * @param node The CAN module instance
 * @return The number of transmission errors
 */
uint8_t canGetErrorCount(canBASE_t *node);

/**
 * @brief Check if CAN messages are pending transmission
 * @param node The CAN module instance
 * @param messageBoxes The bitmask of message boxes to check, set to 0 to check them all
 * @return The bitmask of pending message boxes
 */
uint64_t canIsTxMessagesPending(canBASE_t *node, uint64_t messageBoxes);

#endif /* _CAN_WRAPPERS_H_ */
