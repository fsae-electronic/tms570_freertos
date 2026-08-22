#include <stdlib.h>
#include "dashboard.hpp"
#include "ui_rpm_bar.hpp"
#include "ui_buttons.hpp"
#include "pages.hpp"
#include "ui_touch.hpp"
#include <stdio.h>

#include <math.h>
#include <stdio.h>


dashboard_data_t dashboard_data;

int cal_tps_0_timer = 0;
int cal_tps_100_timer = 0;
int cal_left_steer_timer = 0;
int cal_right_steer_timer = 0;



char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
    char fmt[20];

    // Crear formato tipo "%4.1f"
    sprintf(fmt, "%%%d.%df", width, prec);

    sprintf(sout, fmt, val);

    return sout;
}

void draw_motor_block(
    Bridgetek_EVE2 &eve,
    int x,
    const char *title,
    float voltage,
    float current,
    float temp
)
{
    eve.COLOR_RGB(0,200,255);

    eve.CMD_TEXT(x,100,26,0,title);

    eve.COLOR_RGB(255,255,255);

    eve.CMD_TEXT(x,130,26,0,"V");
    eve.CMD_NUMBER(x+60,130,26,0,voltage);
    

    eve.CMD_TEXT(x,150,26,0,"A");
    eve.CMD_NUMBER(x+60,150,26,0,current);

    eve.CMD_TEXT(x,170,26,0,"T");
    eve.CMD_NUMBER(x+60,170,26,0,temp);

}

void draw_center(Bridgetek_EVE2 &eve, dashboard_data_t *d)
{
    int x = 190;

    eve.CMD_TEXT(x,80,26,0,"TPS");
    eve.CMD_NUMBER(x+60,80,26,0,d->tps);
    eve.CMD_PROGRESS(x,100,100,15,0,d->tps,100);

    eve.CMD_TEXT(x,130,26,0,"FRONT BREAK");
    eve.CMD_NUMBER(x+100,130,26,0,d->brake_front);
    eve.CMD_PROGRESS(x,150,100,15,0,d->brake_front,100);

    eve.CMD_TEXT(x,180,26,0,"REAR BREAK");
    eve.CMD_NUMBER(x+100,180,26,0,d->brake_rear);
    eve.CMD_PROGRESS(x,200,100,15,0,d->brake_rear,100);
}

void draw_status(Bridgetek_EVE2 &eve, dashboard_data_t *d)
{
    // esquina superior derecha
    int x = 340;
    int y = 10;

    // Motor 1 fault
    const char* m1_fault_str;
    switch(d->motor1_fault) {
        case NO_FAULT: m1_fault_str = "OK"; eve.COLOR_RGB(0,255,0); break;
        case OVERVOLTAGE: m1_fault_str = "OVERVOLT"; eve.COLOR_RGB(255,0,0); break;
        case UNDERVOLTAGE: m1_fault_str = "UNDERVOLT"; eve.COLOR_RGB(255,0,0); break;
        case OVERCURRENT: m1_fault_str = "OVERCURR"; eve.COLOR_RGB(255,0,0); break;
        case OVERTEMP: m1_fault_str = "OVERTEMP"; eve.COLOR_RGB(255,0,0); break;
        case INT_ERROR: m1_fault_str = "INT ERR"; eve.COLOR_RGB(255,0,0); break;
        default: m1_fault_str = "UNKNOWN"; eve.COLOR_RGB(255,255,0); break;
    }
    eve.CMD_TEXT(x, y, 22, 0, "M1:");
    eve.CMD_TEXT(x+30, y, 22, 0, m1_fault_str);

    // Motor 2 fault
    const char* m2_fault_str;
    switch(d->motor2_fault) {
        case NO_FAULT: m2_fault_str = "OK"; eve.COLOR_RGB(0,255,0); break;
        case OVERVOLTAGE: m2_fault_str = "OVERVOLT"; eve.COLOR_RGB(255,0,0); break;
        case UNDERVOLTAGE: m2_fault_str = "UNDERVOLT"; eve.COLOR_RGB(255,0,0); break;
        case OVERCURRENT: m2_fault_str = "OVERCURR"; eve.COLOR_RGB(255,0,0); break;
        case OVERTEMP: m2_fault_str = "OVERTEMP"; eve.COLOR_RGB(255,0,0); break;
        case INT_ERROR: m2_fault_str = "INT ERR"; eve.COLOR_RGB(255,0,0); break;
        default: m2_fault_str = "UNKNOWN"; eve.COLOR_RGB(255,255,0); break;
    }
    eve.CMD_TEXT(x, y+20, 22, 0, "M2:");
    eve.CMD_TEXT(x+30, y+20, 22, 0, m2_fault_str);

    // Drive state
    const char* drive_str;
    switch(d->drive_state) {
        case OFF: drive_str = "OFF"; eve.COLOR_RGB(255,255,0); break;
        case ON: drive_str = "ON"; eve.COLOR_RGB(0,255,0); break;
        case FAULT: drive_str = "FAULT"; eve.COLOR_RGB(255,0,0); break;
        default: drive_str = "UNKNOWN"; eve.COLOR_RGB(255,255,0); break;
    }
    eve.CMD_TEXT(x - 60, y, 22, 0, "DRV");
    eve.CMD_TEXT(x - 60, y + 20, 22, 0, drive_str);

    eve.COLOR_RGB(255,255,255); // reset color
}


/**************************************************************************************
 * Functions to initialize and update the dashboard data, and to draw the dashboard   
 * on the EVE display. These functions can be called from the main loop or from a timer  
 * interrupt to keep the dashboard updated with the latest data from the ECU.         
 * Make sure to call update_dashboard_data() after updating the main data fields 
 * to push the latest values into the graph buffers, and then call update_dashboard_draw() 
 * to refresh the display.
 **************************************************************************************
 */

void init_dashboard(dashboard_data_t *data)
{
    data->rpm = 0;

    data->motor1_voltage = 0;
    data->motor1_current = 0;
    data->motor1_temp = 0;

    data->motor2_voltage = 0;
    data->motor2_current = 0;
    data->motor2_temp = 0;

    data->tps = 0;
    data->brake_front = 0;
    data->brake_rear = 0;

    data->battery_voltage = 0;
    data->battery_current = 0;
    data->steering_angle = 0;
    data->wheel_speed_fl = 0;
    data->wheel_speed_fr = 0;
    data->wheel_speed_rl = 0;
    data->wheel_speed_rr = 0;

    data->traction_on = 0;
    data->drive_enabled = 0;

    data->mode = NORMAL; // NORMAL
    data->motor1_fault = NO_FAULT;
    data->motor2_fault = NO_FAULT;
    data->drive_state = OFF;

    data->cal_tps_0 = 0;
    data->cal_tps_100 = 0;
    data->cal_left_steer = 0;
    data->cal_right_steer = 0;

    data->cal_screen = 0;

    // Initialize graph buffers to zero
    for(int i=0; i<GRAPH_BUFFER_SIZE; i++) {
        data->motor1_voltage_history[i] = 0;
        data->motor1_current_history[i] = 0;
        data->motor1_temp_history[i] = 0;
        data->battery_voltage_history[i] = 0;
        data->motor2_voltage_history[i] = 0;
        data->motor2_current_history[i] = 0;
        data->motor2_temp_history[i] = 0;
        data->battery_current_history[i] = 0;
        data->steering_angle_history[i] = 0;
        data->tps_history[i] = 0;
        data->brake_front_history[i] = 0;
        data->brake_rear_history[i] = 0;
        data->wheel_speed_fl_history[i] = 0;
        data->wheel_speed_fr_history[i] = 0;
        data->wheel_speed_rl_history[i] = 0;
        data->wheel_speed_rr_history[i] = 0;
    }
}

void update_dashboard_data(dashboard_data_t *data)
{
    // This function can be called after updating the main data fields to push the latest values into the graph buffers
    data->motor1_voltage_history[data->graph_buffer_index] = data->motor1_voltage;
    data->motor1_current_history[data->graph_buffer_index] = data->motor1_current;
    data->motor1_temp_history[data->graph_buffer_index] = data->motor1_temp;
    data->battery_voltage_history[data->graph_buffer_index] = data->battery_voltage;
    data->motor2_voltage_history[data->graph_buffer_index] = data->motor2_voltage;
    data->motor2_current_history[data->graph_buffer_index] = data->motor2_current;
    data->motor2_temp_history[data->graph_buffer_index] = data->motor2_temp;
    data->battery_current_history[data->graph_buffer_index] = data->battery_current;
    data->steering_angle_history[data->graph_buffer_index] = data->steering_angle;
    data->tps_history[data->graph_buffer_index] = data->tps;
    data->brake_front_history[data->graph_buffer_index] = data->brake_front;
    data->brake_rear_history[data->graph_buffer_index] = data->brake_rear;
    data->wheel_speed_fl_history[data->graph_buffer_index] = data->wheel_speed_fl;
    data->wheel_speed_fr_history[data->graph_buffer_index] = data->wheel_speed_fr;
    data->wheel_speed_rl_history[data->graph_buffer_index] = data->wheel_speed_rl;
    data->wheel_speed_rr_history[data->graph_buffer_index] = data->wheel_speed_rr;

    // Increment buffer index
    data->graph_buffer_index = (data->graph_buffer_index + 1) % GRAPH_BUFFER_SIZE;
}

void update_dashboard_draw(Bridgetek_EVE2 &eve, dashboard_data_t *d)
{
    eve.LIB_BeginCoProList();

    eve.CMD_DLSTART();

    eve.CLEAR_COLOR_RGB(0,0,0);
    eve.CLEAR(1,1,1);

    if(current_page == PAGE_RACE)
    {
        ui_draw_rpm_bar(eve,d->rpm);

        draw_motor_block(
            eve,
            20,
            "MOTOR 1",
            d->motor1_voltage,
            d->motor1_current,
            d->motor1_temp
        );

        draw_motor_block(
            eve,
            360,
            "MOTOR 2",
            d->motor2_voltage,
            d->motor2_current,
            d->motor2_temp
        );

        draw_center(eve,d);

        ui_draw_buttons(eve,d);

        draw_status(eve,d);
    }

    if(current_page == PAGE_TELEMETRY)
    {
        // título
        // dibujar rejilla 4x4
        const int cellW = 115;
        const int cellH = 60;
        int startX = 10;
        int startY = 10;
        eve.COLOR_RGB(255,255,255);
        // helper que maneja negativos mejor que CMD_NUMBER
        auto textInCell = [&](int col,int row,const char* label,float value,const char* unit="", int tag=0){
            int x = startX + col * (cellW + 10);
            int y = startY + row * (cellH + 10);
            eve.TAG(tag);
            eve.BEGIN(eve.BEGIN_RECTS);
            eve.COLOR_RGB(25,25,25);
            eve.VERTEX2F(x*16, y*16);
            eve.VERTEX2F((x+cellW)*16, (y+cellH)*16);
            eve.END();
            eve.COLOR_RGB(255,255,255);
            eve.CMD_TEXT(x+5, y+18, 22, 0, label);
            // convertir a entero con signo
            int iv = (int) value;
            char buf[16];
            sprintf(buf, "%d", iv);
            eve.CMD_TEXT(x+cellW/2, y+18, 22, 0, buf);
            if (unit[0]) eve.CMD_TEXT(x+cellW-20, y+18, 22, 0, unit);
            eve.TAG(255);
        };
        // columna 0: motor1 + battery volt
        textInCell(0,0,"M1 V", d->motor1_voltage, "V", 10);
        textInCell(0,1,"M1 I", d->motor1_current, "A", 11);
        textInCell(0,2,"M1 T", d->motor1_temp, "C", 12);
        textInCell(0,3,"Bat V", d->battery_voltage, "V", 13);
        // columna 1: motor2 + battery current
        textInCell(1,0,"M2 V", d->motor2_voltage, "V", 14);
        textInCell(1,1,"M2 I", d->motor2_current, "A", 15);
        textInCell(1,2,"M2 T", d->motor2_temp, "C", 16);
        textInCell(1,3,"Bat I", d->battery_current, "A", 17);
        // columna 2: controles
        textInCell(2,0,"Steer", d->steering_angle, "deg", 18);
        textInCell(2,1,"TPS", d->tps, "%", 19);
        textInCell(2,2,"F Brk", d->brake_front, "%", 20);
        textInCell(2,3,"R Brk", d->brake_rear, "%", 21);
        // columna 3: wheel speeds
        textInCell(3,0,"FL spd", d->wheel_speed_fl, "kmh", 22);
        textInCell(3,1,"FR spd", d->wheel_speed_fr, "kmh", 23);
        textInCell(3,2,"RL spd", d->wheel_speed_rl, "kmh", 24);
        textInCell(3,3,"RR spd", d->wheel_speed_rr, "kmh", 25);
    }

    if(current_page == PAGE_GRAPH)
    {
        // título del gráfico
        const char* graphTitle = "GRAPH";
        switch(current_graph)
        {
            case GRAPH_M1_V: graphTitle = "Motor 1 Voltage"; break;
            case GRAPH_M1_I: graphTitle = "Motor 1 Current"; break;
            case GRAPH_M1_T: graphTitle = "Motor 1 Temp"; break;
            case GRAPH_BAT_V: graphTitle = "Battery Voltage"; break;
            case GRAPH_M2_V: graphTitle = "Motor 2 Voltage"; break;
            case GRAPH_M2_I: graphTitle = "Motor 2 Current"; break;
            case GRAPH_M2_T: graphTitle = "Motor 2 Temp"; break;
            case GRAPH_BAT_I: graphTitle = "Battery Current"; break;
            case GRAPH_STEER: graphTitle = "Steering Angle"; break;
            case GRAPH_TPS: graphTitle = "TPS"; break;
            case GRAPH_FRONT_BRK: graphTitle = "Front Brake"; break;
            case GRAPH_REAR_BRK: graphTitle = "Rear Brake"; break;
            case GRAPH_FL_SPD: graphTitle = "FL Wheel Speed"; break;
            case GRAPH_FR_SPD: graphTitle = "FR Wheel Speed"; break;
            case GRAPH_RL_SPD: graphTitle = "RL Wheel Speed"; break;
            case GRAPH_RR_SPD: graphTitle = "RR Wheel Speed"; break;
            default: graphTitle = "Unknown"; break;
        }
        eve.CMD_TEXT(240, 20, 26, Bridgetek_EVE2::OPT_CENTER, graphTitle);

        // área del gráfico
        int graphX = 30;
        int graphY = 35;
        int graphW = 420;
        int graphH = 180;

        // fondo del gráfico
        eve.COLOR_RGB(20,20,20);
        eve.BEGIN(eve.BEGIN_RECTS);
        eve.VERTEX2F(graphX*16, graphY*16);
        eve.VERTEX2F((graphX+graphW)*16, (graphY+graphH)*16);
        eve.END();

        // grid
        eve.COLOR_RGB(50,50,50);
        eve.BEGIN(eve.BEGIN_LINES);
        for(int i=0; i<=10; i++)
        {
            int x = graphX + i * (graphW/10);
            eve.VERTEX2F(x*16, graphY*16);
            eve.VERTEX2F(x*16, (graphY+graphH)*16);
        }
        for(int i=0; i<=5; i++)
        {
            int y = graphY + i * (graphH/5);
            eve.VERTEX2F(graphX*16, y*16);
            eve.VERTEX2F((graphX+graphW)*16, y*16);
        }
        eve.END();

        // l�nea del gr�fico (hist�rica)
        eve.COLOR_RGB(0,255,0);
        eve.BEGIN(eve.BEGIN_LINE_STRIP);

        float max_val = 100; // default
        float *history = nullptr;

        switch(current_graph)
        {
            case GRAPH_M1_V: max_val = 500; history = d->motor1_voltage_history; break;
            case GRAPH_M1_I: max_val = 200; history = d->motor1_current_history; break;
            case GRAPH_M1_T: max_val = 100; history = d->motor1_temp_history; break;
            case GRAPH_BAT_V: max_val = 500; history = d->battery_voltage_history; break;
            case GRAPH_M2_V: max_val = 500; history = d->motor2_voltage_history; break;
            case GRAPH_M2_I: max_val = 200; history = d->motor2_current_history; break;
            case GRAPH_M2_T: max_val = 100; history = d->motor2_temp_history; break;
            case GRAPH_BAT_I: max_val = 200; history = d->battery_current_history; break;
            case GRAPH_STEER: max_val = 180; history = d->steering_angle_history; break;
            case GRAPH_TPS: max_val = 100; history = d->tps_history; break;
            case GRAPH_FRONT_BRK: max_val = 100; history = d->brake_front_history; break;
            case GRAPH_REAR_BRK: max_val = 100; history = d->brake_rear_history; break;
            case GRAPH_FL_SPD: max_val = 300; history = d->wheel_speed_fl_history; break;
            case GRAPH_FR_SPD: max_val = 300; history = d->wheel_speed_fr_history; break;
            case GRAPH_RL_SPD: max_val = 300; history = d->wheel_speed_rl_history; break;
            case GRAPH_RR_SPD: max_val = 300; history = d->wheel_speed_rr_history; break;
            default: history = nullptr; break;
        }

        if(history)
        {
            for(int i = 0; i < GRAPH_BUFFER_SIZE; i++)
            {
                int idx = (d->graph_buffer_index + i) % GRAPH_BUFFER_SIZE;

                float val = history[idx];

                if(current_graph == GRAPH_STEER)
                    val = fabs(val);

                // limitar valores
                if(val < 0) val = 0;
                if(val > max_val) val = max_val;

                // FIX divisi�n entera
                int x = graphX + (i * graphW) / (GRAPH_BUFFER_SIZE - 1);

                float norm = val / max_val;

                int y = graphY + graphH - (int)(norm * graphH);

                eve.VERTEX2F(x * 16, y * 16);
            }
        }

        eve.END();

        // mostrar valor actual
        float current_val = 0;
        switch(current_graph)
        {
            case GRAPH_M1_V: current_val = d->motor1_voltage; break;
            case GRAPH_M1_I: current_val = d->motor1_current; break;
            case GRAPH_M1_T: current_val = d->motor1_temp; break;
            case GRAPH_BAT_V: current_val = d->battery_voltage; break;
            case GRAPH_M2_V: current_val = d->motor2_voltage; break;
            case GRAPH_M2_I: current_val = d->motor2_current; break;
            case GRAPH_M2_T: current_val = d->motor2_temp; break;
            case GRAPH_BAT_I: current_val = d->battery_current; break;
            case GRAPH_STEER: current_val = abs(d->steering_angle); break;
            case GRAPH_TPS: current_val = d->tps; break;
            case GRAPH_FRONT_BRK: current_val = d->brake_front; break;
            case GRAPH_REAR_BRK: current_val = d->brake_rear; break;
            case GRAPH_FL_SPD: current_val = d->wheel_speed_fl; break;
            case GRAPH_FR_SPD: current_val = d->wheel_speed_fr; break;
            case GRAPH_RL_SPD: current_val = d->wheel_speed_rl; break;
            case GRAPH_RR_SPD: current_val = d->wheel_speed_rr; break;
            default: current_val = 0; break;
        }
        int y_val = graphY + graphH - (current_val / max_val * graphH);
        char val_str[16];
        dtostrf(current_val, 4, 1, val_str);
        eve.CMD_TEXT(graphX + graphW + 10, y_val, 26, 0, val_str);

        // escala Y
        eve.COLOR_RGB(255,255,255);
        char scale_str[16];
        dtostrf(max_val, 4, 0, scale_str);
        eve.CMD_TEXT(graphX - 35, graphY, 22, 0, scale_str);
        dtostrf(max_val/2, 4, 0, scale_str);
        eve.CMD_TEXT(graphX - 35, graphY + graphH/2, 22, 0, scale_str);
        eve.CMD_TEXT(graphX - 10, graphY + graphH, 22, 0, "0");

        // botón back
        eve.TAG(30);
        eve.COLOR_RGB(100,100,100);
        eve.CMD_BUTTON(200, 235, 80, 30, 26, 0, "BACK");
        eve.TAG(255);
    }

    if(current_page == PAGE_DEBUG)
    {
        // título
        eve.CMD_TEXT(240, 20, 22, Bridgetek_EVE2::OPT_CENTER, "DEBUG MENU");


        static int counter = 0;
        

// fila 1: calibraciones TPS
        eve.TAG(40);

        if(d->cal_tps_0)
        {
            eve.COLOR_RGB(0,0,255);
            eve.CMD_BUTTON(20, 40, 120, 40, 26, EVE_OPT_FLAT, "Send");

            if(cal_tps_0_timer > 0)
            {
                cal_tps_0_timer--;
            }
            else
            {
                d->cal_tps_0 = 0;
            }
        }
        else
        {
            eve.COLOR_RGB(255,255,255);
            eve.CMD_BUTTON(20, 40, 120, 40, 26, 0, "Cal TPS 0%");
        }

        eve.TAG(41);
        if(d->cal_tps_100)
        {
            eve.COLOR_RGB(0,0,255);
            eve.CMD_BUTTON(170, 40, 120, 40, 26, EVE_OPT_FLAT, "Send");

            if(cal_tps_100_timer > 0)
            {
                cal_tps_100_timer--;
            }
            else
            {
                d->cal_tps_100 = 0;
            }
        }
        else
        {
            eve.COLOR_RGB(255,255,255);
            eve.CMD_BUTTON(170, 40, 120, 40, 26, 0, "Cal TPS 100%");
        }
        
        
        // fila 2: modos
        if (d->traction_on)
        {
            eve.TAG(42);
            eve.COLOR_RGB(0,255,0);
            eve.CMD_BUTTON(320, 40, 120, 40, 26, EVE_OPT_FLAT, "Traction: ON");
        }
        else
        {
            eve.TAG(42);
            eve.COLOR_RGB(255,0,0);
            eve.CMD_BUTTON(320, 40, 120, 40, 26, 0, "Traction: OFF");
        }

        if(d->mode == RACE)
        {
            eve.TAG(43);
            eve.COLOR_RGB(255,0,0);
            eve.CMD_BUTTON(20, 100, 120, 40, 26, EVE_OPT_FLAT, "Mode: RACE");
        }else
        {             
            eve.TAG(43);
            eve.COLOR_RGB(0,255,0);
            eve.CMD_BUTTON(20, 100, 120, 40, 26, 0, "Mode: NORMAL");
        }
        
        if(d->drive_enabled)
        {
            eve.TAG(44);
            eve.COLOR_RGB(0,255,0);
            eve.CMD_BUTTON(170, 100, 120, 40, 26, EVE_OPT_FLAT, "Drive: ON");
        }else
        {
            eve.TAG(44);
            eve.COLOR_RGB(255,0,0);
            eve.CMD_BUTTON(170, 100, 120, 40, 26, 0, "Drive: OFF");
        }

        if(d->telemetry_enabled)
        {
            eve.TAG(45);
            eve.COLOR_RGB(0,255,0);
            eve.CMD_BUTTON(320, 100, 120, 40, 26, EVE_OPT_FLAT, "Telemetry: ON");
        }else
        {
            eve.TAG(45);
            eve.COLOR_RGB(255,0,0);
            eve.CMD_BUTTON(320, 100, 120, 40, 26, 0, "Telemetry: OFF");
        }

        // fila 3: steering
        eve.TAG(46);
        if (d->cal_left_steer)
        {
            eve.COLOR_RGB(0,0,255);
            eve.CMD_BUTTON(20, 160, 120, 40, 26, EVE_OPT_FLAT, "Send");
            if(cal_left_steer_timer > 0)
            {
                cal_left_steer_timer--;
            }
            else
            {
                d->cal_left_steer = 0;
            }
        }else
        {
            eve.COLOR_RGB(255,255,255);
            eve.CMD_BUTTON(20, 160, 120, 40, 26, 0, "Cal LEFT");
        }


        eve.TAG(47);
        if(d->cal_right_steer)
        {
            eve.COLOR_RGB(0,0,255);
            eve.CMD_BUTTON(170, 160, 120, 40, 26, EVE_OPT_FLAT, "Send");
            if(cal_right_steer_timer > 0)
            {
                cal_right_steer_timer--;
            }
            else
            {
                d->cal_right_steer = 0;
            }
        }else
        {
            eve.COLOR_RGB(255,255,255);
            eve.CMD_BUTTON(170, 160, 120, 40, 26, 0, "Cal RIGHT");
        }
        



        // fila 4: calibrate screen
        if(d->cal_screen)
        {
            eve.TAG(49);
            eve.COLOR_RGB(0,0,255);
            eve.CMD_BUTTON(20, 220, 120, 40, 26, EVE_OPT_FLAT, "Calibrating...");
        }else
        {
            eve.TAG(49);
            eve.COLOR_RGB(255,255,255);
            eve.CMD_BUTTON(20, 220, 120, 40, 26, 0, "Cal Screen");
        }

        // eve.TAG(50);
        // eve.CMD_BUTTON(170, 220, 120, 40, 26, 0, "");
        // eve.TAG(51);
        // eve.CMD_BUTTON(320, 220, 120, 40, 26, 0, "");

        eve.TAG(255);
    }

    eve.DISPLAY();

    eve.CMD_SWAP();

    eve.LIB_EndCoProList();
}