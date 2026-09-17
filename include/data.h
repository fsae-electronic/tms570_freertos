#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>

/* CANOpen states */
typedef enum
{
    BOOTUP = 0x00,
    OPERATIONAL = 0x05,
    PRE_OPERATIONAL = 0x7F,
    STOPPED = 0x04
} CANOpenState_t;

/* Driver error codes */
typedef enum
{
    NO_FAULT = 0x0000,
    ERROR_CURRENT_A = 0xFF01,
    ERROR_CURRENT_B,
    ERROR_HS_FET,
    ERROR_LS_FET,
    ERROR_DRV_LS_L1,
    ERROR_DRV_LS_L2,
    ERROR_DRV_LS_L3,
    ERROR_DRV_HS_L1,
    ERROR_DRV_HS_L2,
    ERROR_DRV_HS_L3,
    ERROR_MOTOR_FEEDBACK,
    ERROR_DC_LINK_UNDERVOLTAGE,
    ERROR_PULS_MODE_FINISHED,
    ERROR_APP_ERROR,
    ERROR_STO_ERROR,
    ERROR_CONTROLLER_OVERTEMPERATURE,
    ERROR_DC_LINK_OVERVOLTAGE = 0x3210
} DriverErrorCode_t;

typedef enum
{
    CALIB_CMD_NONE,
    CALIB_CMD_TPS_0,
    CALIB_CMD_TPS_100,
    CALIB_CMD_LEFT_STEER,
    CALIB_CMD_CENTER_STEER,
    CALIB_CMD_RIGHT_STEER,
    CALIB_CMD_CURRENT_SENSORS
} CalibrationCmd_t;

/**************************************************************************************
 *                                                                                    *
 * Mailboxes                                                                          *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Front ADC2CAN                                                                      *
 *                                                                                    *
 * ID 0x501: TPS Data                                                                 *
 * b0-1: TPS 1                                                                        *
 * b2-3: TPS 2                                                                        *
 * b4-7: Free                                                                         *
 *                                                                                    *
 * ID 0x502: Front Data                                                               *
 * b0-1: Front Left Speed                                                             *
 * b2-3: Front Right Speed                                                            *
 * b4-5: Direction Angle                                                              *
 * b6-7: Front Brake Pressure                                                         *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Rear ADC2CAN                                                                       *
 *                                                                                    *
 * ID 0x503: Current Data                                                             *
 * b0-1: AC Current Driver 1                                                          *
 * b2-3: AC Current Driver 2                                                          *
 * b4-5: DC Current Driver 1                                                          *
 * b6-7: DC Current Driver 2                                                          *
 *                                                                                    *
 * ID 0x504: Rear Data                                                                *
 * b0-1: Rear Brake Pressure                                                          *
 * b2-7: Free                                                                         *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Main ECU                                                                           *
 *                                                                                    *
 * ID 0x401: Main ECU Data                                                            *
 * b0: TPS                                                                            *
 * b1: Mode                                                                           *
 * b2: Error                                                                          *
 * b3-7: TBD                                                                          *
 *                                                                                    *
 * ID 0x600: Calibration Message                                                      *
 * b0: -                                                                              *
 * b1: TPS 0%                                                                         *
 * b2: TPS 100%                                                                       *
 * b3: Steering Left                                                                  *
 * b4: Steering Center                                                                *
 * b5: Steering Right                                                                 *
 * b6: Current Sensors                                                                *
 * b7: Free                                                                           *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Drivers                                                                            *
 *                                                                                    *
 * ID 0x700: Heartbeat                                                                *
 * b0: CANOpen State                                                                  *
 * b1-7: Free                                                                         *
 *                                                                                    *
 * ID 0x200+Node_ID: Driver Control                                                   *
 * b0-1: Control Word                                                                 *
 * b2-3: Target Torque                                                                *
 * b4-7: Free                                                                         *
 *                                                                                    *
 * ID 0x180+Node_ID: Driver Status                                                    *
 * b0-1: Status Word                                                                  *
 * b2-3: Reserved                                                                     *
 * b4-5: Warnings                                                                     *
 * b6-7: Errors                                                                       *
 *                                                                                    *
 * ID 0x280+Node_ID: Motor Data                                                       *
 * b0-1: Motor Velocity                                                               *
 * b2-5: Motor Rated Current                                                          *
 * b6: Motor Temp                                                                     *
 * b7: Free                                                                           *
 *                                                                                    *
 * ID 0x380+Node_ID: Driver Data                                                      *
 * b0: Driver Temp                                                                    *
 * b1-2: Voltage DC Battery                                                           *
 * b3-7: Free                                                                         *
 *                                                                                    *
 **************************************************************************************/

typedef enum
{
    DATA_ID_TPS = 0x501,
    DATA_ID_FRONT = 0x502,
    DATA_ID_CURRENT = 0x503,
    DATA_ID_REAR = 0x504,
    DATA_ID_MAIN_ECU = 0x401,
    DATA_ID_CALIB = 0x600,
    DATA_ID_HEARTBEAT = 0x700,
    DATA_ID_DRV_CTRL1 = 0x201,
    DATA_ID_DRV_CTRL2 = 0x202,
    DATA_ID_DRV_STATUS1 = 0x181,
    DATA_ID_DRV_STATUS2 = 0x182,
    DATA_ID_MOTOR1 = 0x281,
    DATA_ID_MOTOR2 = 0x282,
    DATA_ID_DRV1 = 0x381,
    DATA_ID_DRV2 = 0x382
} DataID_t;

// typedef union
// {
//     uint8_t raw[4];

//     struct
//     {
//         uint16_t xTPS1;
//         uint16_t xTPS2;
//     } values;
// } DataTPS_t;

typedef struct
{
    // uint8_t raw[4];

    uint16_t xTPS1;
    uint16_t xTPS2;
} DataTPS_t;

typedef struct
{
    // uint8_t raw[8];

    uint16_t xFLSpeed;
    uint16_t xFRSpeed;
    uint16_t xDirAngle;
    uint16_t xFBrake;
} DataFront_t;

typedef struct
{
    // uint8_t raw[8];

    uint16_t xAC1;
    uint16_t xAC2;
    uint16_t xDC1;
    uint16_t xDC2;
} DataCurrent_t;

typedef struct
{
    // uint8_t raw[2];

    uint16_t xRBrake;
} DataRear_t;

typedef struct
{
    // uint8_t raw[6];

    uint8_t xTPS;
    uint8_t xMode;
    uint8_t xError;
} DataMainECU_t;

typedef struct
{
    // uint8_t raw[7];

    // uint8_t x_;
    // uint8_t xTPS0;
    // uint8_t xTPS100;
    // uint8_t xSteeringL;
    // uint8_t xSteeringC;
    // uint8_t xSteeringR;
    // uint8_t xCurrent;

    CalibrationCmd_t xCmd;
} DataCalib_t;

typedef struct
{
    // uint8_t raw[1];

    uint8_t xCANOpenState;
} DataHeartbeat_t;

typedef struct
{
    // uint8_t raw[4];

    uint16_t xCtrlWord;
    uint16_t xTargetTorque;
} DataDrvCtrl_t;

typedef struct
{
    // uint8_t raw[8];

    union
    {
        uint16_t xStatusWord;
        struct
        {
            uint16_t xReady : 1;
            uint16_t xSwitchedOn : 1;
            uint16_t xOpEnabled : 1;
            uint16_t xFault : 1;
            uint16_t xVoltageEnabled : 1;
            uint16_t xQuickStop : 1;
            uint16_t xSwitchOnDisabled : 1;
            uint16_t xWarning : 1;
            uint16_t xReserved : 1;
            uint16_t xTargetReached : 1;
            uint16_t xLimitActive : 1;
        } xStatusBits;
    };
    uint16_t xReserved;
    union
    {
        uint16_t xWarnings;
        struct
        {
            uint16_t xControllerTemp : 1;
            uint16_t xMotorTemp : 1;
            uint16_t xDCUnderVoltage : 1;
            uint16_t xDCOverVoltage : 1;
            uint16_t xDCOverCurrent : 1;
            uint16_t xStallProtection : 1;
            uint16_t xMaxVelExceeded : 1;
            uint16_t xBMSProposedPwr : 1;
            uint16_t xCapacitorTemp : 1;
            uint16_t xI2TProtection : 1;
            uint16_t xFieldWeakening : 1;
        } xWarningBits;
    };
    uint16_t xErrors;
} DataDrvStatus_t;

typedef struct
{
    // uint8_t raw[7];

    uint16_t xMotorVel;
    uint32_t xMotorCurrent;
    uint8_t xMotorTemp;
} DataMotor_t;

typedef struct
{
    // uint8_t raw[3];

    uint8_t xDrvTemp;
    uint16_t xVoltageDCBat;
} DataDrv_t;


/**
 * @brief Pack/Unpack raw data from/into a structured format based on the data ID.
 * @param raw Pointer to the raw data.
 * @param values Pointer to the structured data.
 * @param id The data ID.
 * @note Prevents Alignment Fault / Data Abort errors and avoids performance penalties
 *       associated with unaligned memory access.
 */
void dataPack(uint8_t *raw, void *values, DataID_t id);
void dataUnpack(uint8_t *raw, void *values, DataID_t id);

#endif /* _DATA_H_ */
