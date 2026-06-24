#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>

/* CANOpen states */
#define BOOTUP 0x00
#define OPERATIONAL 0x05
#define PRE_OPERATIONAL 0x7F
#define STOPPED 0x04

/* Driver error codes */
#define NO_FAULT 0x0000
#define ERROR_CURRENT_A 0xFF01
#define ERROR_CURRENT_B 0xFF02
#define ERROR_HS_FET 0xFF03
#define ERROR_LS_FET 0xFF04
#define ERROR_DRV_LS_L1 0xFF05
#define ERROR_DRV_LS_L2 0xFF06
#define ERROR_DRV_LS_L3 0xFF07
#define ERROR_DRV_HS_L1 0xFF08
#define ERROR_DRV_HS_L2 0xFF09
#define ERROR_DRV_HS_L3 0xFF0A
#define ERROR_MOTOR_FEEDBACK 0xFF0B
#define ERROR_DC_LINK_UNDERVOLTAGE 0xFF0C
#define ERROR_PULS_MODE_FINISHED 0xFF0D
#define ERROR_APP_ERROR 0xFF0E
#define ERROR_STO_ERROR 0xFF0F
#define ERROR_CONTROLLER_OVERTEMPERATURE 0xFF10
#define ERROR_DC_LINK_OVERVOLTAGE 0x3210

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

typedef struct
{
    union
    {
        uint8_t raw[4];

        struct
        {
            uint16_t xTPS1;
            uint16_t xTPS2;
        } values;
    };
} DataTPS_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t xFLSpeed;
            uint16_t xFRSpeed;
            uint16_t xDirAngle;
            uint16_t xFBrake;
        } values;
    };
} DataFront_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t xAC1;
            uint16_t xAC2;
            uint16_t xDC1;
            uint16_t xDC2;
        } values;
    };
} DataCurrent_t;

typedef struct
{
    union
    {
        uint8_t raw[2];

        struct
        {
            uint16_t xRBrake;
        } values;
    };
} DataRear_t;

typedef struct
{
    union
    {
        uint8_t raw[6];

        struct
        {
            uint8_t xTPS;
            uint8_t xMode;
            uint8_t xError;
        } values;
    };
} DataMainECU_t;

typedef struct
{
    union
    {
        uint8_t raw[7];

        struct
        {
            uint8_t x_;
            uint8_t xTPS0;
            uint8_t xTPS100;
            uint8_t xSteeringL;
            uint8_t xSteeringC;
            uint8_t xSteeringR;
            uint8_t xCurrent;
        } values;
    };
} DataCalib_t;

typedef struct
{
    union
    {
        uint8_t raw[1];

        struct
        {
            uint8_t xCANOpenState;
        } values;
    };
} DataHeartbeat_t;

typedef struct
{
    union
    {
        uint8_t raw[4];

        struct
        {
            uint16_t xCtrlWord;
            uint16_t xTargetTorque;
        } values;
    };
} DataDrvCtrl_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
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
        } values;
    };
} DataDrvStatus_t;

typedef struct
{
    union
    {
        uint8_t raw[7];

        struct
        {
            uint16_t xMotorVel;
            uint32_t xMotorCurrent;
            uint8_t xMotorTemp;
        } values;
    };
} DataMotor_t;

typedef struct
{
    union
    {
        uint8_t raw[3];

        struct
        {
            uint8_t xDrvTemp;
            uint16_t xVoltageDCBat;
        } values;
    };
} DataDrv_t;

#endif /* _DATA_H_ */
