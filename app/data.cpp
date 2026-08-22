#include "data.hpp"
#include "dashboard.hpp"

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
    canEnableloopback(canREG1, Internal_Lbk); // Enable loopback mode for testing without actual CAN hardware



    sciInit();
    sciSetBaudrate(sciREG, 115200U);

    const char *msg = "TMS570 ON-BOARD ECU INITIALIZED\n";
    sciSend(sciREG, strlen(msg), (uint8*)msg);
}


void update_data(void)
{
    // This function can be used to perform any periodic updates or checks for the drivers if needed
    // Send data of buttons status to dashboard
    if(sensors_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX1, sensors_data.raw);
        dashboard_data.tps = (float)sensors_data.values.tps;
        dashboard_data.brake_front = (float)sensors_data.values.brake_front;
        dashboard_data.brake_rear = (float)sensors_data.values.brake_rear;
        dashboard_data.steering_angle = (float)sensors_data.values.steering_angle;
        sensors_data.new_data = false;
    }
    if(speed_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX2, speed_data.raw);
        dashboard_data.wheel_speed_fl = (float)speed_data.values.wheel_speed_fl;
        dashboard_data.wheel_speed_fr = (float)speed_data.values.wheel_speed_fr;
        dashboard_data.wheel_speed_rl = (float)speed_data.values.wheel_speed_rl;
        dashboard_data.wheel_speed_rr = (float)speed_data.values.wheel_speed_rr;
        speed_data.new_data = false;
    }
    if(motor1_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX3, motor1_data.raw);
        dashboard_data.motor1_voltage = (float)motor1_data.values.motor_voltage;
        dashboard_data.motor1_current = (float)motor1_data.values.motor_current;
        dashboard_data.motor1_temp = (float)motor1_data.values.motor_temp;
        motor1_data.new_data = false;
    }
    if(motor2_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX4, motor2_data.raw);
        dashboard_data.motor2_voltage = (float)motor2_data.values.motor_voltage;
        dashboard_data.motor2_current = (float)motor2_data.values.motor_current;
        dashboard_data.motor2_temp = (float)motor2_data.values.motor_temp;
        motor2_data.new_data = false;
    }
    if(drivers_status_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX5, drivers_status_data.raw);
        dashboard_data.motor1_fault = drivers_status_data.values.motor1_fault;
        dashboard_data.motor2_fault = drivers_status_data.values.motor2_fault;
        dashboard_data.drive_state = drivers_status_data.values.drive_state;
        drivers_status_data.new_data = false;
    }
    if(dynamic_data.new_data)
    {
        canGetData(canREG1, canMESSAGE_BOX6, dynamic_data.raw);
        dashboard_data.rpm = (float)dynamic_data.values.rpm;
        dashboard_data.battery_voltage = (float)dynamic_data.values.battey_voltage;
        dashboard_data.battery_current = (float)dynamic_data.values.battery_current;
        dynamic_data.new_data = false;
    }


    // Send buttons status to dashboard
    buttons_data.values.drive_enabled = dashboard_data.drive_enabled;
    buttons_data.values.traction_on = dashboard_data.traction_on;
    buttons_data.values.telemetry_enabled = dashboard_data.telemetry_enabled;
    buttons_data.values.mode = dashboard_data.mode;
    canTransmit(canREG1, canMESSAGE_BOX9, buttons_data.raw);

    // Send on-board ECU status to dashboard
    on_board_status_data.values.on_board_ok = 1; // Assuming the ECU is always OK for this example
    on_board_status_data.values.can_ok = 1; // Assuming CAN is always OK for this example
    canTransmit(canREG1, canMESSAGE_BOX10, on_board_status_data.raw);

}

// CAN message notification callback
// This function will be called by the CAN driver when a new message is 
// received in one of the configured message boxes




