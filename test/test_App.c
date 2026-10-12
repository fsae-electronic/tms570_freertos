#include <string.h>

#include "can.h"
#include "sci.h"

#include "data.h"
#include "test_App.h"

#include "msgpack/cwpack.h"

// Simulated values
static float tps_1 = 0;
static float tps_2 = 0;

static float fl_speed = 0;
static float fr_speed = 0;
static float dir_angle = 0;
static float f_brake = 0;

static float ac_1 = 0;
static float ac_2 = 0;
static float dc_1 = 0;
static float dc_2 = 0;

static float r_brake = 0;

static float tps_main = 0;
static float mode_main = 0;
static float error_main = 0;

static float motor_vel_1 = 0;
static float motor_vel_2 = 0;
static float motor_current_1 = 20;
static float motor_current_2 = 20;
static float motor_temp_1 = 40;
static float motor_temp_2 = 40;

static float drv_temp_1 = 40;
static float drv_temp_2 = 40;
static float battery_voltage_1 = 400;
static float battery_voltage_2 = 400;

static uint32_t sim_counter = 0;

// static uint8_t sci_tx_status_frame[] = {
//     0x7EU, 0x00U, 0x0FU,
//     0x8BU, 0x01U,
//     0x00U, 0x13U, 0xA2U, 0x00U, 0x40U, 0xB2U, 0xB3U, 0xD4U,
//     0xFFU, 0xFEU,
//     0x00U, 0x00U, 0x00U,
//     0x48U
// };

static int iMsgPackTest(void)
{
    uint8_t uTxBuff[256]; // Buffer for msgpack data
    const char *str = "Hello";
    cw_pack_context pc;
    cw_unpack_context uc;

    /* Initialize the packer with the buffer */
    cw_pack_context_init(&pc, uTxBuff, sizeof(uTxBuff), NULL);

    /* Serialize a structured payload (array containing [42, true, "Hello"]) */
    cw_pack_array_size(&pc, 3); // Tell the packer an array of 3 elements follows
    cw_pack_signed(&pc, 42);
    cw_pack_true(&pc);
    cw_pack_str(&pc, str, strlen(str) - 1); // Exclude null terminator

    if (pc.return_code != CWP_RC_OK)
        return pc.return_code;

    cw_unpack_context_init(&uc, uTxBuff, sizeof(uTxBuff), NULL);

    cw_unpack_next(&uc);
    cw_unpack_next(&uc);
    cw_unpack_next(&uc);
    cw_unpack_next(&uc);
    cw_unpack_next(&uc); // Should return CWP_RC_END_OF_INPUT

    return uc.return_code;
}

void vTestAppInit(void)
{
    /* Enable loopback mode for testing without actual hardware */
    canEnableloopback(canREG1, Internal_Lbk);
    // sciEnableLoopback(sciREG, Digital_Lbk);

    // iMsgPackTest();
}

void vTestAppPeriodic(void)
{
    /* Buffers */
    DataTPS_t tps_msg;
    DataFront_t front_msg;
    DataCurrent_t current_msg;
    DataRear_t rear_msg;
    DataMainECU_t main_ecu_msg;
    DataMotor_t motor_msg_1, motor_msg_2;
    DataDrv_t drv_msg_1, drv_msg_2;
    uint8_t raw[8];

    /* Simulate sensor data changes */

    tps_1 += 1;
    if (tps_1 > 100)
        tps_1 = 0;
    tps_2 += 1;
    if (tps_2 > 100)
        tps_2 = 0;

    fl_speed += 1;
    if (fl_speed > 250)
        fl_speed = 0;
    fr_speed += 1.2;
    if (fr_speed > 250)
        fr_speed = 0;
    dir_angle += 1;
    if (dir_angle > 90)
        dir_angle = -90;
    f_brake += 0.5;
    if (f_brake > 100)
        f_brake = 0;

    ac_1 += 0.2;
    if (ac_1 > 100)
        ac_1 = 0;
    ac_2 += 0.3;
    if (ac_2 > 100)
        ac_2 = 0;
    dc_1 += 0.4;
    if (dc_1 > 100)
        dc_1 = 0;
    dc_2 += 0.5;
    if (dc_2 > 100)
        dc_2 = 0;

    r_brake += 0.3;
    if (r_brake > 100)
        r_brake = 0;

    motor_vel_1 += 5;
    if (motor_vel_1 > 5000)
        motor_vel_1 = 0;
    motor_current_1 += 0.05;
    if (motor_current_1 > 60)
        motor_current_1 = 40;
    motor_temp_1 += 0.1;
    if (motor_temp_1 > 90)
        motor_temp_1 = 40;
    motor_vel_2 += 10;
    if (motor_vel_2 > 5000)
        motor_vel_2 = 0;
    motor_current_2 += 0.1;
    if (motor_current_2 > 60)
        motor_current_2 = 40;
    motor_temp_2 += 0.2;
    if (motor_temp_2 > 90)
        motor_temp_2 = 40;

    drv_temp_1 += 0.1;
    if (drv_temp_1 > 90)
        drv_temp_1 = 40;
    drv_temp_2 += 0.2;
    if (drv_temp_2 > 90)
        drv_temp_2 = 40;
    battery_voltage_1 += 0.5;
    if (battery_voltage_1 > 400)
        battery_voltage_1 = 350;
    battery_voltage_2 += 1;
    if (battery_voltage_2 > 400)
        battery_voltage_2 = 350;

    /* Simulate faults and drive state changes */

    sim_counter++;

    if ((sim_counter % 1000) == 0)
    {
    }

    /* Store the data in the message structures */

    tps_msg.xTPS1 = tps_1;
    tps_msg.xTPS2 = tps_2;

    front_msg.xFLSpeed = fl_speed;
    front_msg.xFRSpeed = fr_speed;
    front_msg.xDirAngle = dir_angle + 180; // avoid negatives
    front_msg.xFBrake = f_brake;

    current_msg.xAC1 = ac_1;
    current_msg.xAC2 = ac_2;
    current_msg.xDC1 = dc_1;
    current_msg.xDC2 = dc_2;

    rear_msg.xRBrake = r_brake;

    motor_msg_1.xMotorVel = motor_vel_1;
    motor_msg_1.xMotorCurrent = motor_current_1;
    motor_msg_1.xMotorTemp = motor_temp_1;
    motor_msg_2.xMotorVel = motor_vel_2;
    motor_msg_2.xMotorCurrent = motor_current_2;
    motor_msg_2.xMotorTemp = motor_temp_2;

    drv_msg_1.xDrvTemp = drv_temp_1;
    drv_msg_1.xVoltageDCBat = battery_voltage_1;
    drv_msg_2.xDrvTemp = drv_temp_2;
    drv_msg_2.xVoltageDCBat = battery_voltage_2;

    /* Pack data into CAN messages and transmit them */

    dataPack(raw, &tps_msg, DATA_ID_TPS);
    canTransmit(canREG1, canMESSAGE_BOX1, raw);
    dataPack(raw, &front_msg, DATA_ID_FRONT);
    canTransmit(canREG1, canMESSAGE_BOX2, raw);
    dataPack(raw, &current_msg, DATA_ID_CURRENT);
    canTransmit(canREG1, canMESSAGE_BOX3, raw);
    dataPack(raw, &rear_msg, DATA_ID_REAR);
    canTransmit(canREG1, canMESSAGE_BOX4, raw);
    dataPack(raw, &motor_msg_1, DATA_ID_MOTOR1);
    canTransmit(canREG1, canMESSAGE_BOX12, raw);
    dataPack(raw, &motor_msg_2, DATA_ID_MOTOR2);
    canTransmit(canREG1, canMESSAGE_BOX13, raw);
    dataPack(raw, &drv_msg_1, DATA_ID_DRV1);
    canTransmit(canREG1, canMESSAGE_BOX14, raw);
    dataPack(raw, &drv_msg_2, DATA_ID_DRV2);
    canTransmit(canREG1, canMESSAGE_BOX15, raw);

    /* Receive CAN messages from Main ECU */

    // if (canIsRxMessageArrived(canREG1, canMESSAGE_BOX5))
    // {
    //     canGetData(canREG1, canMESSAGE_BOX5, raw);
    //     dataUnpack(raw, &main_ecu_msg, DATA_ID_MAIN_ECU);

    //     /* Process Main ECU message */
    // }
}
