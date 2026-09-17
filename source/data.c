#include "data.h"

void dataPack(uint8_t *raw, void *values, DataID_t id)
{
    if (raw != NULL && values != NULL)
    {
        switch (id)
        {
        case DATA_ID_MAIN_ECU:
            raw[0] = ((DataMainECU_t *)values)->xTPS;
            raw[1] = ((DataMainECU_t *)values)->xMode;
            raw[2] = ((DataMainECU_t *)values)->xError;
            break;
        case DATA_ID_CALIB:
            raw[0] = ((DataCalib_t *)values)->xCmd;
            break;
        case DATA_ID_DRV1:
        case DATA_ID_DRV2:
            raw[0] = ((DataDrv_t *)values)->xDrvTemp;
            raw[1] = ((DataDrv_t *)values)->xVoltageDCBat & 0xFF;
            raw[2] = ((DataDrv_t *)values)->xVoltageDCBat >> 8;
            break;
        default:
            // Ignore unknown data ID
            break;
        }
    }
}

void dataUnpack(uint8_t *raw, void *values, DataID_t id)
{
    if (raw != NULL && values != NULL)
    {
        switch (id)
        {
        case DATA_ID_TPS:
            ((DataTPS_t *)values)->xTPS1 = raw[0] | (raw[1] << 8);
            ((DataTPS_t *)values)->xTPS2 = raw[2] | (raw[3] << 8);
            break;
        case DATA_ID_FRONT:
            ((DataFront_t *)values)->xFLSpeed = raw[0] | (raw[1] << 8);
            ((DataFront_t *)values)->xFRSpeed = raw[2] | (raw[3] << 8);
            ((DataFront_t *)values)->xDirAngle = raw[4] | (raw[5] << 8);
            ((DataFront_t *)values)->xFBrake = raw[6] | (raw[7] << 8);
            break;
        case DATA_ID_CURRENT:
            ((DataCurrent_t *)values)->xAC1 = raw[0] | (raw[1] << 8);
            ((DataCurrent_t *)values)->xAC2 = raw[2] | (raw[3] << 8);
            ((DataCurrent_t *)values)->xDC1 = raw[4] | (raw[5] << 8);
            ((DataCurrent_t *)values)->xDC2 = raw[6] | (raw[7] << 8);
            break;
        case DATA_ID_REAR:
            ((DataRear_t *)values)->xRBrake = raw[0] | (raw[1] << 8);
            break;
        case DATA_ID_HEARTBEAT:
            ((DataHeartbeat_t *)values)->xCANOpenState = raw[0];
            break;
        case DATA_ID_DRV_STATUS1:
        case DATA_ID_DRV_STATUS2:
            ((DataDrvStatus_t *)values)->xStatusWord = raw[0] | (raw[1] << 8);
            ((DataDrvStatus_t *)values)->xReserved = raw[2] | (raw[3] << 8);
            ((DataDrvStatus_t *)values)->xWarnings = raw[4] | (raw[5] << 8);
            ((DataDrvStatus_t *)values)->xErrors = raw[6] | (raw[7] << 8);
            break;
        case DATA_ID_MOTOR1:
        case DATA_ID_MOTOR2:
            ((DataMotor_t *)values)->xMotorVel = raw[0] | (raw[1] << 8);
            ((DataMotor_t *)values)->xMotorCurrent = raw[2] | (raw[3] << 8) | (raw[4] << 16) | (raw[5] << 24);
            ((DataMotor_t *)values)->xMotorTemp = raw[6];
            break;
        case DATA_ID_DRV1:
        case DATA_ID_DRV2:
            ((DataDrv_t *)values)->xDrvTemp = raw[0];
            ((DataDrv_t *)values)->xVoltageDCBat = raw[1] | (raw[2] << 8);
            break;
        default:
            // Ignore unknown data ID
            break;
        }
    }
}
