/*
 * data.h
 *
 *  Created on: 13 mar. 2026
 *      Author: agust
 */

#ifndef SOURCE_DATA_H_
#define SOURCE_DATA_H_

#include <stdint.h>
#include <stdbool.h>
/* Sensors and actuators IDs

Pedals: 0x120 b0-1: TPS1 ; b2-3: TPS2 ; b4-5: Front brake ; b6-7: Rear brake
Steering wheel: 0x121 b0-1: Steering angle
Speed: 0x122 b0-1: FR ; b2-3: FL ; b4-5: RR ; b6-7: RL
Current: 0x123 b0-1: AC_D1 ; b2-3: AC_D2 ; DC_D1 ; b4-5: DC_D2 ; b6-7: DC_D3

*/

#define SENSORS_ID 0x120
#define WHEEL_SPEED_ID 0x121
#define MOTOR1_ID 0x122
#define MOTOR2_ID 0x123
#define STATUS_DRIVERS_ID 0x124
#define DYNAMIC_ID 0x125

#define BUTTONS_ID 0x200
#define STATUS_ON_BOARD_ECU_ID 0x201


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t tps;
            uint16_t brake_front;
            uint16_t brake_rear;
            uint16_t steering_angle;
        } values;
    };
    volatile bool new_data;
} sensors_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t wheel_speed_fl; // Front right wheel speed
            uint16_t wheel_speed_fr; // Front left wheel speed
            uint16_t wheel_speed_rl; // Rear left wheel speed
            uint16_t wheel_speed_rr; // Rear right wheel speed
        } values;
    };
    volatile bool new_data;
} speed_data_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t rpm; // Engine RPM
            uint16_t battey_voltage; // Battery voltage
            uint16_t battery_current; // Battery current
        } values;
    };
    volatile bool new_data;
} dynamic_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t motor_voltage; // AC current sensor 1
            uint16_t motor_current; // AC current sensor 2
            uint16_t motor_temp; // DC current sensor 1
        } values;
    };
    volatile bool new_data;
} motor_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t motor1_fault; // Motor 1 fault -> motor_fault_t
            uint8_t motor2_fault; // Motor 2 fault
            uint8_t drive_state; // bits 2-3: Drive state (0 = OFF, 1 = ON, 2 = FAULT)
        } values;
    };
    volatile bool new_data;
} drivers_status_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t drive_enabled; // bit 0: Drive enabled
            uint8_t traction_on;   // bit 1: Traction control on
            uint8_t mode;          // bit 2: Mode (0 = Normal, 1 = Race)
            uint8_t telemetry_enabled; // bit 3: Telemetry enabled
        } values;
    };
    volatile bool new_data;
} buttons_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t on_board_ok;
            uint8_t can_ok;
        } values;
    };
    volatile bool new_data;
} on_board_status_data_t;


extern sensors_data_t sensors_data;
extern speed_data_t speed_data;
extern dynamic_data_t dynamic_data;
extern motor_data_t motor1_data;
extern motor_data_t motor2_data;
extern drivers_status_data_t drivers_status_data;
extern buttons_data_t buttons_data;
extern on_board_status_data_t on_board_status_data;



void init_data(void);
void update_data(void);

#endif /* SOURCE_DATA_H_ */