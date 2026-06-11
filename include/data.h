#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>

/**************************************************************************************
 *                                                                                    *
 * Mailboxes                                                                          *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Front ADC2CAN                                                                      *
 *                                                                                    *
 * ID 0x501:                                                                          *
 * b0-1: apps 1                                                                       *
 * b2-3: apps 2                                                                       *
 * b4-5: front brake                                                                  *
 *                                                                                    *
 * ID 0x502:                                                                          *
 * b0-1: front wheel 1                                                                *
 * b2-3: front wheel 2                                                                *
 * b4-5: steering                                                                     *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Rear ADC2CAN                                                                       *
 *                                                                                    *
 * ID 0x503:                                                                          *
 * b0-1: ac 1                                                                         *
 * b2-3: ac 2                                                                         *
 * b4-5: dc 1                                                                         *
 * b6-7: dc 2                                                                         *
 *                                                                                    *
 * ID 0x504:                                                                          *
 * b0-1: rear brake                                                                   *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Main ECU                                                                           *
 *                                                                                    *
 * ID 0x505:                                                                          *
 * b0-1: drivers status                                                               *
 *                                                                                    *
 * ID 0x506:                                                                          *
 * b0-1: error msgs                                                                   *
 *                                                                                    *
 * ID 0x60n:                                                                          *
 * Sensor calibration mapped to 0x50n messages                                        *
 *                                                                                    *
 **************************************************************************************
 *                                                                                    *
 * Drivers                                                                            *
 *                                                                                    *
 * ID 0x200+Node_ID:                                                                  *
 * b0: ControlWord                                                                    *
 * b1: Node_ID                                                                        *
 * b2-5: Target Velocity                                                              *
 * b6-7: Target Torque                                                                *
 *                                                                                    *
 * ID 0x507:                                                                          *
 * b0-1: speed N1                                                                     *
 * b2-3: speed N2                                                                     *
 * b4-5: battery                                                                      *
 *                                                                                    *
 * ID 0x508:                                                                          *
 * b0-1: motor temp N1                                                                *
 * b2-3: motor temp N2                                                                *
 * b4-5: driver temp N1                                                               *
 * b6-7: driver temp N2                                                               *
 *                                                                                    *
 **************************************************************************************/

typedef struct
{
    union
    {
        uint8_t raw[6];

        struct
        {
            uint16_t xAPPS1;
            uint16_t xAPPS2;
            uint16_t xFBrake;
        } values;
    };
} DataFADC1_t;

typedef struct
{
    union
    {
        uint8_t raw[6];

        struct
        {
            uint16_t xFWheel1;
            uint16_t xFWheel2;
            uint16_t xSteering;
        } values;
    };
} DataFADC2_t;

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
} DataRADC1_t;

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
} DataRADC2_t;

typedef struct
{
    union
    {
        uint8_t raw[2];

        struct
        {
            uint16_t xDrvStatus;
        } values;
    };
} DataDrvStatus_t;

typedef struct
{
    union
    {
        uint8_t raw[2];

        struct
        {
            uint16_t xErrMsg;
        } values;
    };
} DataErrMsg_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t xControlWord;
            uint8_t xNodeID;
            uint32_t xTargetVelocity;
            uint16_t xTargetTorque;
        } values;
    };
} DataDrvCtrl_t;

typedef struct
{
    union
    {
        uint8_t raw[6];

        struct
        {
            uint16_t xSpeedN1;
            uint16_t xSpeedN2;
            uint16_t xBattery;
        } values;
    };
} DataDrv1_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t xMotorTempN1;
            uint16_t xMotorTempN2;
            uint16_t xDriverTempN1;
            uint16_t xDriverTempN2;
        } values;
    };
} DataDrv2_t;

#endif /* _DATA_H_ */
