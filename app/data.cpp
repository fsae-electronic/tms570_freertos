#include "data.hpp"

#include "can.h"
#include "sci.h"
#include <string.h>


sensors_data_t sensors_data;
speed_data_t speed_data;
dynamic_data_t dynamic_data;
motor_data_t motor1_data;
motor_data_t motor2_data;
drivers_status_data_t drivers_status_data;
buttons_data_t buttons_data;
on_board_status_data_t on_board_status_data;


void init_data(void)
{
    canInit();
    // NOTE: internal loopback removed -- CAN1 now listens on the real bus
    // (needed to receive frames from the adc2can_front sensor board).

    sciInit();
    sciSetBaudrate(sciREG, 115200U);

    const char *msg = "TMS570 MAIN ECU INITIALIZED\n";
    sciSend(sciREG, strlen(msg), (uint8*)msg);
}


void update_data(void)
{
    // Drain whichever mailboxes have a new frame pending. The converted
    // values live in the raw CAN structs themselves (sensors_data,
    // speed_data, ...); vTelemetryTask reads those directly through the
    // AppData_t snapshot -- there is no separate display-facing struct.
    if(sensors_data.new_data)
    {
        // CAN ID 0x501 (adc2can_front): TPS1/TPS2.
        canGetData(canREG1, canMESSAGE_BOX1, sensors_data.raw);
        sensors_data.new_data = false;
    }
    if(speed_data.new_data)
    {
        // CAN ID 0x502 (adc2can_front): front axle only -- no rear wheel
        // speed/brake data exists yet (no rear sensor board on the bus).
        canGetData(canREG1, canMESSAGE_BOX2, speed_data.raw);
        speed_data.new_data = false;
    }
    if(motor1_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX3, motor1_data.raw);
        motor1_data.new_data = false;
    }
    if(motor2_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX4, motor2_data.raw);
        motor2_data.new_data = false;
    }
    if(drivers_status_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX5, drivers_status_data.raw);
        drivers_status_data.new_data = false;
    }
    if(dynamic_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX6, dynamic_data.raw);
        dynamic_data.new_data = false;
    }

    // Send buttons status (no touchscreen to drive these -- always the
    // zero-initialized default until a real button/switch input exists).
    canTransmit(canREG1, canMESSAGE_BOX9, buttons_data.raw);

    // Send on-board ECU status
    on_board_status_data.values.on_board_ok = 1; // Assuming the ECU is always OK for this example
    on_board_status_data.values.can_ok = 1; // Assuming CAN is always OK for this example
    canTransmit(canREG1, canMESSAGE_BOX10, on_board_status_data.raw);
}

// CAN message notification callback
// This function will be called by the CAN driver when a new message is 
// received in one of the configured message boxes




